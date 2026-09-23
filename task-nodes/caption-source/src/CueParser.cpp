#include "CueParser.h"

#include "TimingLineParser.h"
#include "TimestampParser.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <sstream>
#include <string>

namespace edgeai
{
namespace
{

bool isNumeric(const std::string& value)
{
    if (value.empty())
    {
        return false;
    }

    for (const unsigned char character : value)
    {
        if (std::isdigit(character) == 0)
        {
            return false;
        }
    }

    return true;
}

bool looksLikeTimingLine(const std::string& line)
{
    return line.find("-->") != std::string::npos;
}

bool parseIndex(const std::string& value, std::uint64_t& out)
{
    const char* begin = value.data();
    const char* end = value.data() + value.size();

    const auto result = std::from_chars(begin, end, out);

    return result.ec == std::errc{} && result.ptr == end;
}

CueParseResult failure(
    const CueParseError error,
    std::string message)
{
    CueParseResult result;
    result.success = false;
    result.error = error;
    result.message = std::move(message);
    return result;
}

} // namespace

CueParseResult CueParser::parse(
    const CueBlock& block,
    const std::uint64_t sourceOrder)
{
    if (block.lines.empty())
    {
        return failure(
            CueParseError::EmptyBlock,
            "Cue block contains no lines");
    }

    std::size_t timingLineIndex = 0;
    std::uint64_t sourceIndex = 0;
    bool hasIndex = false;

    // Decide whether line 0 is an index or the timing line itself.
    if (looksLikeTimingLine(block.lines[0]))
    {
        // Format B: timing line first, no index.
        timingLineIndex = 0;
        hasIndex = false;
    }
    else if (isNumeric(block.lines[0]))
    {
        // Format A: numeric index on line 0, timing on line 1.
        if (block.lines.size() < 2)
        {
            return failure(
                CueParseError::MissingTimingLine,
                "Cue block has an index but no timing line");
        }

        parseIndex(block.lines[0], sourceIndex);
        hasIndex = true;
        timingLineIndex = 1;
    }
    else
    {
        // Line 0 is neither numeric nor a timing line.
        // Try to recover if line 1 is a timing line.
        if (block.lines.size() >= 2 &&
            looksLikeTimingLine(block.lines[1]))
        {
            timingLineIndex = 1;
            hasIndex = false;

            // Non-numeric index is recoverable but worth flagging.
            CueParseResult recovered =
                parseFromTimingLine(block, timingLineIndex, 0, sourceOrder);

            if (recovered.success &&
                recovered.error == CueParseError::None)
            {
                recovered.error = CueParseError::InvalidIndex;
                recovered.message =
                    "Non-numeric index line was ignored";
            }

            return recovered;
        }

        return failure(
            CueParseError::MissingTimingLine,
            "Cue block does not contain a recognizable timing line");
    }

    return parseFromTimingLine(
        block,
        timingLineIndex,
        hasIndex ? sourceIndex : 0,
        sourceOrder);
}

CueParseResult CueParser::parseFromTimingLine(
    const CueBlock& block,
    const std::size_t timingLineIndex,
    const std::uint64_t sourceIndex,
    const std::uint64_t sourceOrder)
{
    const TimingLineParseResult timing =
        TimingLineParser::parse(block.lines[timingLineIndex]);

    if (!timing.success)
    {
        return failure(
            CueParseError::InvalidTimingLine,
            "Invalid timing line: " + timing.message);
    }

    // Text is everything after the timing line.
    const std::size_t textStart = timingLineIndex + 1;

    std::ostringstream textBuilder;
    bool firstTextLine = true;

    for (std::size_t i = textStart; i < block.lines.size(); ++i)
    {
        if (!firstTextLine)
        {
            textBuilder << '\n';
        }

        textBuilder << block.lines[i];
        firstTextLine = false;
    }

    std::string text = textBuilder.str();

    if (text.empty())
    {
        return failure(
            CueParseError::MissingText,
            "Cue block has valid timing but no text");
    }

    CaptionCue cue;
    cue.source_order = sourceOrder;
    cue.index = sourceIndex;
    // TimingInterval is in microsecond Pts units; the orchestrator's
    // CaptionCue.start_ms/end_ms contract (see CaptionSourceNode.h) is milliseconds.
    cue.start_ms = timing.interval.start_pts / kPtsPerMillisecond;
    cue.end_ms = timing.interval.end_pts / kPtsPerMillisecond;
    cue.text = std::move(text);

    CueParseResult result;
    result.success = true;
    result.cue = std::move(cue);
    result.error = CueParseError::None;
    return result;
}

} // namespace edgeai
