#pragma once

#include <cstdint>
#include <string>

namespace edgeai
{

enum class SpeakerGender
{
    Unknown,
    Male,
    Female,
    Neutral
};

struct CueMetadata
{
    std::string language;
    std::string speaker_id;
    std::string speaker_name;

    SpeakerGender gender{
        SpeakerGender::Unknown
    };
};

// One normalized caption cue, format-agnostic (fed by a .srt sidecar parser today;
// same struct will be fed by an in-band CEA-608/708/WebVTT tap point later).
struct CaptionCue
{
    uint64_t index{0};

    uint64_t source_order{0};

    int64_t start_ms{0};
    int64_t end_ms{0};

    std::string text;

    CueMetadata metadata;
};

} // namespace edgeai
