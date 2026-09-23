#pragma once

#include "CueBlock.h"
#include "CaptionCue.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace edgeai
{

enum class CueParseError
{
    None,
    EmptyBlock,
    MissingTimingLine,
    InvalidTimingLine,
    MissingText,
    InvalidIndex
};

struct CueParseResult
{
    bool success{false};
    CaptionCue cue;
    CueParseError error{CueParseError::None};
    std::string message;
};

class CueParser
{
public:
    static CueParseResult parse(
        const CueBlock& block,
        std::uint64_t sourceOrder);

private:
    static CueParseResult parseFromTimingLine(
        const CueBlock& block,
        std::size_t timingLineIndex,
        std::uint64_t sourceIndex,
        std::uint64_t sourceOrder);
};

} // namespace edgeai
