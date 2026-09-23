#include "CaptionSourceNode.h"

#include "CueParser.h"
#include "SrtBlockReader.h"
#include "MetadataExtractor.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

namespace edgeai
{
namespace
{

IssueCode mapCueParseError(
    const CueParseError error)
{
    switch (error)
    {
        case CueParseError::EmptyBlock:
            return IssueCode::EmptyCue;

        case CueParseError::MissingTimingLine:
        case CueParseError::InvalidTimingLine:
            return IssueCode::InvalidTiming;

        case CueParseError::MissingText:
            return IssueCode::EmptyCue;

        case CueParseError::InvalidIndex:
            return IssueCode::InvalidIndex;

        case CueParseError::None:
        default:
            return IssueCode::MalformedCue;
    }
}

IssueSeverity severityForCueError(
    const CueParseError error)
{
    switch (error)
    {
        case CueParseError::MissingTimingLine:
        case CueParseError::InvalidTimingLine:
            return IssueSeverity::Error;

        case CueParseError::EmptyBlock:
        case CueParseError::MissingText:
        case CueParseError::InvalidIndex:
            return IssueSeverity::Warning;

        case CueParseError::None:
        default:
            return IssueSeverity::Warning;
    }
}

std::string describeCue(
    const CaptionCue& cue)
{
    std::ostringstream stream;

    stream
        << "index=" << cue.index
        << ", source_order=" << cue.source_order
        << ", start_ms=" << cue.start_ms
        << ", end_ms=" << cue.end_ms;

    return stream.str();
}

} // namespace

ParseResult CaptionSourceNode::parseSrtFile(
    const std::filesystem::path& filePath)
{
    ParseResult result;

    std::ifstream input(filePath);

    if (!input.is_open())
    {
        ParseIssue issue;

        issue.severity =
            IssueSeverity::Error;

        issue.code =
            IssueCode::FileOpenFailed;

        issue.line_number = 0;

        issue.message =
            "Failed to open SRT file: " +
            filePath.string();

        result.issues.push_back(
            std::move(issue));

        return result;
    }

    return parse(input);
}

ParseResult CaptionSourceNode::parse(
    std::istream& stream)
{
    ParseResult result;

    auto blocks =
        SrtBlockReader::readBlocks(stream);

    result.blocks_read =
        blocks.size();

    // Build the extractor once, only if enabled.
    const bool doExtract = mOptions.extract_metadata;

    MetadataExtractorOptions extractorOptions;
    extractorOptions.extract_speaker = true;
    extractorOptions.default_language = mOptions.default_language;

    const MetadataExtractor extractor(extractorOptions);

    std::uint64_t sourceOrder = 0;

    for (const auto& block : blocks)
    {
        CueParseResult cueResult =
            CueParser::parse(
                block,
                sourceOrder);

        ++sourceOrder;

        if (cueResult.success)
        {
            // Optional, opt-in metadata enrichment.
            if (doExtract)
            {
                extractor.apply(cueResult.cue);
            }

            result.cues.push_back(
                std::move(cueResult.cue));

            ++result.cues_accepted;

            continue;
        }

        ++result.cues_rejected;

        ParseIssue issue;

        issue.severity =
            severityForCueError(
                cueResult.error);

        issue.code =
            mapCueParseError(
                cueResult.error);

        issue.line_number =
            block.first_line_number;

        issue.message =
            cueResult.message.empty()
                ? "Failed to parse SRT cue block"
                : cueResult.message;

        result.issues.push_back(
            std::move(issue));
    }

    validateSourceOrder(result);   // detect non-monotonic source + per-cue sanity
    sortCues(result);              // guarantee monotonic output
    validateTimeline(result);      // detect real overlaps on sorted cues

    return result;
}

void CaptionSourceNode::validateSourceOrder(ParseResult& result)
{
    for (std::size_t index = 0; index < result.cues.size(); ++index)
    {
        const CaptionCue& current = result.cues[index];

        // Per-cue sanity (order-independent). Normally caught earlier,
        // but kept here as defensive validation.
        if (current.end_ms < current.start_ms)
        {
            ParseIssue issue;
            issue.severity = IssueSeverity::Error;
            issue.code     = IssueCode::EndBeforeStart;
            issue.line_number = 0;
            issue.message =
                "Cue end PTS is earlier than start PTS: " +
                describeCue(current);
            result.issues.push_back(std::move(issue));
        }
        else if (current.end_ms == current.start_ms)
        {
            ParseIssue issue;
            issue.severity = IssueSeverity::Warning;
            issue.code     = IssueCode::ZeroDuration;
            issue.line_number = 0;
            issue.message =
                "Cue has zero duration: " + describeCue(current);
            result.issues.push_back(std::move(issue));
        }

        if (index == 0)
        {
            continue;
        }

        const CaptionCue& previous = result.cues[index - 1];

        // Non-monotonic is a property of SOURCE order -> check before sort.
        if (current.start_ms < previous.start_ms)
        {
            ParseIssue issue;
            issue.severity = IssueSeverity::Warning;
            issue.code     = IssueCode::NonMonotonicTiming;
            issue.line_number = 0;
            issue.message =
                "Cue starts before the preceding source cue. "
                "Previous cue: " + describeCue(previous) +
                "; current cue: " + describeCue(current);
            result.issues.push_back(std::move(issue));
        }
    }
}

void CaptionSourceNode::validateTimeline(ParseResult& result)
{
    // Overlap/adjacency is a property of the TIMELINE -> check after sort.
    // Half-open intervals [start, end): start == previous.end is adjacency.
    for (std::size_t index = 1; index < result.cues.size(); ++index)
    {
        const CaptionCue& previous = result.cues[index - 1];
        const CaptionCue& current  = result.cues[index];

        if (current.start_ms < previous.end_ms)
        {
            ParseIssue issue;
            issue.severity = IssueSeverity::Warning;
            issue.code     = IssueCode::OverlappingCue;
            issue.line_number = 0;
            issue.message =
                "Cue overlaps the preceding cue on the timeline. "
                "Previous cue: " + describeCue(previous) +
                "; current cue: " + describeCue(current);
            result.issues.push_back(std::move(issue));
        }
    }
}

void CaptionSourceNode::sortCues(
    ParseResult& result)
{
    std::stable_sort(
        result.cues.begin(),
        result.cues.end(),
        [](const CaptionCue& left,
           const CaptionCue& right)
        {
            if (left.start_ms != right.start_ms)
            {
                return left.start_ms <
                       right.start_ms;
            }

            if (left.end_ms != right.end_ms)
            {
                return left.end_ms <
                       right.end_ms;
            }

            return left.source_order <
                   right.source_order;
        });
}

} // namespace edgeai

