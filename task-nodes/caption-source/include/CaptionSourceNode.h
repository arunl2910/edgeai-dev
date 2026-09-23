#pragma once

#include "ParseResult.h"

#include <filesystem>
#include <istream>
#include <string>
#include <utility>

namespace edgeai
{

struct ParserOptions
{
    // When true, a "SPEAKER: text" prefix is extracted into
    // cue.metadata.speaker_name and stripped from the text.
    bool extract_metadata{false};

    // Applied to every cue's metadata.language when non-empty.
    // Language comes from track/catalog context, never inferred from text.
    std::string default_language;
};

// Reads a .srt sidecar file and produces an ordered list of CaptionCue.
// Parsing core for AI-37-05.
class CaptionSourceNode
{
public:
    CaptionSourceNode() = default;

    explicit CaptionSourceNode(ParserOptions options)
        : mOptions(std::move(options))
    {
    }

    [[nodiscard]]
    ParseResult parseSrtFile(
        const std::filesystem::path& filePath);

    [[nodiscard]]
    ParseResult parse(
        std::istream& stream);

private:
    static void validateSourceOrder(ParseResult& result);  // before sort
    static void sortCues(ParseResult& result);             // stable sort
    static void validateTimeline(ParseResult& result);     // after sort

    ParserOptions mOptions;
};

} // namespace edgeai

