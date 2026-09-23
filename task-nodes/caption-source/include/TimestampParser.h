#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace edgeai
{

using Pts = std::int64_t;

inline constexpr Pts kPtsPerMillisecond = 1'000;
inline constexpr Pts kPtsPerSecond = 1'000'000;
inline constexpr Pts kPtsPerMinute = 60 * kPtsPerSecond;
inline constexpr Pts kPtsPerHour = 60 * kPtsPerMinute;

enum class TimestampParseMode
{
    Strict,
    Lenient
};

enum class TimestampError
{
    None,
    Empty,
    InvalidFormat,
    InvalidHours,
    InvalidMinutes,
    InvalidSeconds,
    InvalidMilliseconds,
    OutOfRange,
    Overflow
};

struct TimestampParseResult
{
    bool success{false};
    Pts pts{0};

    TimestampError error{TimestampError::None};
    std::string message;

    // True when lenient mode accepted a non-standard form.
    bool normalized{false};
};

class TimestampParser
{
public:
    [[nodiscard]]
    static TimestampParseResult parse(
        std::string_view input,
        TimestampParseMode mode = TimestampParseMode::Lenient);
};

} // namespace edgeai
