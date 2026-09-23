// TimingLineParser.h

#pragma once

#include "TimingInterval.h"

#include <string>
#include <string_view>

namespace edgeai
{

enum class TimingLineError
{
    None,
    MissingArrow,
    InvalidStartTimestamp,
    InvalidEndTimestamp,
    EndBeforeStart,
    ZeroDuration
};

struct TimingLineParseResult
{
    bool success{false};

    TimingInterval interval;

    TimingLineError error{
        TimingLineError::None
    };

    std::string message;
};

class TimingLineParser
{
public:

    static TimingLineParseResult parse(
        std::string_view line);
};

} // namespace edgeai
