#pragma once

#include "CaptionCue.h"
#include <set>
#include <string>

namespace edgeai
{

struct MetadataExtractorOptions
{
    // Recognise a leading "SPEAKER: text" prefix and move the name
    // into cue.metadata.speaker_name, stripping it from the text.
    bool extract_speaker{true};

    // Apply a default language to every cue (from track/catalog context).
    // Empty means "leave language untouched".
    std::string default_language;

    // Speaker name is only accepted if it is at most this many characters
    // and matches a conservative pattern. Prevents treating ordinary
    // sentences like "Warning: ..." as a speaker.
    std::size_t max_speaker_name_length{24};

    // When non-empty, ONLY these names are treated as speakers.
    // This resolves the "Warning:" vs "Proog:" ambiguity using
    // track/catalog context instead of guessing.
    std::set<std::string> known_speakers;
};

class MetadataExtractor
{
public:
    explicit MetadataExtractor(MetadataExtractorOptions options = {});

    // Enriches a single cue in place.
    void apply(CaptionCue& cue) const;

private:
    MetadataExtractorOptions mOptions;
};

} // namespace edgeai
