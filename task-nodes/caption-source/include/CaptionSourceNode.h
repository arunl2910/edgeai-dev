#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace edgeai {

// One normalized caption cue, format-agnostic (fed by a .srt sidecar parser today;
// same struct will be fed by an in-band CEA-608/708/WebVTT tap point later).
struct CaptionCue
{
    int index{0};
    int64_t start_ms{0};
    int64_t end_ms{0};
    std::string speaker;   // empty if none detected
    std::string text;
};

// Reads a .srt sidecar file and produces an ordered list of CaptionCue.
// Parsing core for AI-37-05. Not implemented yet (stub).
class CaptionSourceNode
{
public:
    std::vector<CaptionCue> parseSrtFile(const std::string& path) const;
};

} // namespace edgeai
