#include "TimestampParser.h"

#include <charconv>
#include <cctype>
#include <limits>
#include <string_view>

namespace edgeai
{
namespace
{

std::string_view trim(std::string_view value)
{
    while (!value.empty() &&
           std::isspace(static_cast<unsigned char>(value.front())))
    {
        value.remove_prefix(1);
    }

    while (!value.empty() &&
           std::isspace(static_cast<unsigned char>(value.back())))
    {
        value.remove_suffix(1);
    }

    return value;
}

bool parseUnsigned(
    std::string_view input,
    std::uint64_t& output)
{
    if (input.empty())
    {
        return false;
    }

    for (const char character : input)
    {
        if (!std::isdigit(static_cast<unsigned char>(character)))
        {
            return false;
        }
    }

    const char* begin = input.data();
    const char* end = input.data() + input.size();

    const auto result = std::from_chars(begin, end, output);

    return result.ec == std::errc{} &&
           result.ptr == end;
}

TimestampParseResult failure(
    TimestampError error,
    std::string message)
{
    TimestampParseResult result;
    result.success = false;
    result.error = error;
    result.message = std::move(message);
    return result;
}

bool safeMultiplyAndAdd(
    std::uint64_t value,
    std::uint64_t multiplier,
    std::uint64_t& total)
{
    const auto maximum =
        static_cast<std::uint64_t>(
            std::numeric_limits<Pts>::max());

    if (value > maximum / multiplier)
    {
        return false;
    }

    const std::uint64_t converted = value * multiplier;

    if (total > maximum - converted)
    {
        return false;
    }

    total += converted;
    return true;
}

} // namespace

TimestampParseResult TimestampParser::parse(
    std::string_view input,
    TimestampParseMode mode)
{
    input = trim(input);

    if (input.empty())
    {
        return failure(
            TimestampError::Empty,
            "Timestamp is empty");
    }

    const std::size_t firstColon = input.find(':');

    if (firstColon == std::string_view::npos)
    {
        return failure(
            TimestampError::InvalidFormat,
            "Timestamp does not contain the first ':' separator");
    }

    const std::size_t secondColon =
        input.find(':', firstColon + 1);

    if (secondColon == std::string_view::npos)
    {
        return failure(
            TimestampError::InvalidFormat,
            "Timestamp does not contain the second ':' separator");
    }

    const std::size_t comma =
        input.find(',', secondColon + 1);

    const std::size_t period =
        input.find('.', secondColon + 1);

    std::size_t millisecondSeparator =
        std::string_view::npos;

    bool normalized = false;

    if (comma != std::string_view::npos)
    {
        millisecondSeparator = comma;
    }
    else if (mode == TimestampParseMode::Lenient &&
             period != std::string_view::npos)
    {
        millisecondSeparator = period;
        normalized = true;
    }
    else
    {
        return failure(
            TimestampError::InvalidFormat,
            "Timestamp must contain a ',' before milliseconds");
    }

    if (input.find(':', secondColon + 1) !=
        std::string_view::npos)
    {
        return failure(
            TimestampError::InvalidFormat,
            "Timestamp contains too many ':' separators");
    }

    const std::string_view hoursText =
        input.substr(0, firstColon);

    const std::string_view minutesText =
        input.substr(
            firstColon + 1,
            secondColon - firstColon - 1);

    const std::string_view secondsText =
        input.substr(
            secondColon + 1,
            millisecondSeparator - secondColon - 1);

    const std::string_view millisecondsText =
        input.substr(millisecondSeparator + 1);

    if (mode == TimestampParseMode::Strict)
    {
        if (hoursText.size() != 2 ||
            minutesText.size() != 2 ||
            secondsText.size() != 2 ||
            millisecondsText.size() != 3)
        {
            return failure(
                TimestampError::InvalidFormat,
                "Strict timestamp must use HH:MM:SS,mmm");
        }
    }
    else
    {
        if (hoursText.size() != 2 ||
            minutesText.size() != 2 ||
            secondsText.size() != 2 ||
            millisecondsText.size() != 3)
        {
            normalized = true;
        }
    }

    std::uint64_t hours = 0;
    std::uint64_t minutes = 0;
    std::uint64_t seconds = 0;
    std::uint64_t milliseconds = 0;

    if (!parseUnsigned(hoursText, hours))
    {
        return failure(
            TimestampError::InvalidHours,
            "Hours field is not an unsigned integer");
    }

    if (!parseUnsigned(minutesText, minutes))
    {
        return failure(
            TimestampError::InvalidMinutes,
            "Minutes field is not an unsigned integer");
    }

    if (!parseUnsigned(secondsText, seconds))
    {
        return failure(
            TimestampError::InvalidSeconds,
            "Seconds field is not an unsigned integer");
    }

    if (!parseUnsigned(millisecondsText, milliseconds))
    {
        return failure(
            TimestampError::InvalidMilliseconds,
            "Milliseconds field is not an unsigned integer");
    }

    if (minutes > 59)
    {
        return failure(
            TimestampError::OutOfRange,
            "Minutes field must be between 0 and 59");
    }

    if (seconds > 59)
    {
        return failure(
            TimestampError::OutOfRange,
            "Seconds field must be between 0 and 59");
    }

    if (mode == TimestampParseMode::Lenient)
    {
        if (millisecondsText.size() == 1)
        {
            milliseconds *= 100;
        }
        else if (millisecondsText.size() == 2)
        {
            milliseconds *= 10;
        }
        else if (millisecondsText.size() != 3)
        {
            return failure(
                TimestampError::InvalidMilliseconds,
                "Milliseconds must contain one to three digits");
        }
    }

    if (milliseconds > 999)
    {
        return failure(
            TimestampError::OutOfRange,
            "Milliseconds field must be between 0 and 999");
    }

    std::uint64_t total = 0;

    if (!safeMultiplyAndAdd(
            hours,
            static_cast<std::uint64_t>(kPtsPerHour),
            total) ||
        !safeMultiplyAndAdd(
            minutes,
            static_cast<std::uint64_t>(kPtsPerMinute),
            total) ||
        !safeMultiplyAndAdd(
            seconds,
            static_cast<std::uint64_t>(kPtsPerSecond),
            total) ||
        !safeMultiplyAndAdd(
            milliseconds,
            static_cast<std::uint64_t>(kPtsPerMillisecond),
            total))
    {
        return failure(
            TimestampError::Overflow,
            "Timestamp exceeds the supported PTS range");
    }

    TimestampParseResult result;
    result.success = true;
    result.pts = static_cast<Pts>(total);
    result.error = TimestampError::None;
    result.normalized = normalized;

    if (normalized)
    {
        result.message =
            "Timestamp was accepted using lenient normalization";
    }

    return result;
}

} // namespace edgeai
