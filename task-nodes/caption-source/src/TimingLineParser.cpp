#include "TimingLineParser.h"

#include "TimestampParser.h"

#include <cctype>
#include <string_view>

namespace edgeai
{
namespace
{

constexpr std::string_view kArrow = "-->";

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

TimingLineParseResult failure(
    const TimingLineError error,
    std::string message)
{
    TimingLineParseResult result;
    result.success = false;
    result.error = error;
    result.message = std::move(message);
    return result;
}

} // namespace

TimingLineParseResult TimingLineParser::parse(
    std::string_view line)
{
    line = trim(line);

    const std::size_t arrowPos = line.find(kArrow);

    if (arrowPos == std::string_view::npos)
    {
        return failure(
            TimingLineError::MissingArrow,
            "Timing line does not contain the '-->' separator");
    }

    // Guard against a second arrow, which indicates a malformed line.
    if (line.find(kArrow, arrowPos + kArrow.size()) !=
        std::string_view::npos)
    {
        return failure(
            TimingLineError::MissingArrow,
            "Timing line contains more than one '-->' separator");
    }

    const std::string_view startText =
        trim(line.substr(0, arrowPos));

    const std::string_view endText =
        trim(line.substr(arrowPos + kArrow.size()));

    const TimestampParseResult startResult =
        TimestampParser::parse(startText);

    if (!startResult.success)
    {
        return failure(
            TimingLineError::InvalidStartTimestamp,
            "Invalid start timestamp: " + startResult.message);
    }

    const TimestampParseResult endResult =
        TimestampParser::parse(endText);

    if (!endResult.success)
    {
        return failure(
            TimingLineError::InvalidEndTimestamp,
            "Invalid end timestamp: " + endResult.message);
    }

    if (endResult.pts < startResult.pts)
    {
        return failure(
            TimingLineError::EndBeforeStart,
            "End timestamp is earlier than start timestamp");
    }

    TimingLineParseResult result;
    result.success = true;
    result.interval.start_pts = startResult.pts;
    result.interval.end_pts = endResult.pts;

    // Zero-duration is accepted but flagged so the caller can decide.
    if (endResult.pts == startResult.pts)
    {
        result.error = TimingLineError::ZeroDuration;
        result.message = "Cue has zero duration";
    }

    return result;
}

} // namespace edgeai
