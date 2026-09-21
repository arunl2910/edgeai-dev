#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "CaptionSourceNode.h"

namespace edgeai {

struct Sentence
{
    std::string text;
    int64_t start_ms{0};
    int64_t end_ms{0};
    std::vector<int> source_cue_indices;
};

// Reconstructs whole sentences from caption cue fragments, flushing on sentence-ending
// punctuation or a speaker change. Not implemented yet (stub).
class DefragNode
{
public:
    std::vector<Sentence> defrag(const std::vector<CaptionCue>& cues) const;
};

} // namespace edgeai
