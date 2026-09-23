#include "MetadataExtractor.h"

#include <cctype>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace edgeai
{
namespace
{

std::string_view trim(std::string_view value)
{
    while (!value.empty() &&
           std::isspace(static_cast<unsigned char>(value.front())))
    {
        value.remove_prefix(1);
    }

    while (!value.empty() &&
           std::isspace(static_cast<unsigned char>(value.back())))
    {
        value.remove_suffix(1);
    }

    return value;
}

bool isPlausibleSpeaker(std::string_view name, std::size_t maxLength)
{
    if (name.empty() || name.size() > maxLength)
    {
        return false;
    }

    if (std::isalpha(static_cast<unsigned char>(name.front())) == 0)
    {
        return false;
    }

    for (const char character : name)
    {
        const unsigned char c = static_cast<unsigned char>(character);

        const bool ok =
            std::isalnum(c) != 0 ||
            c == ' ' ||
            c == '_' ||
            c == '-';

        if (!ok)
        {
            return false;
        }
    }

    return true;
}

// Splits "Proog: Hello there" into ("Proog", "Hello there").
// Returns false if the line has no plausible "SPEAKER: " prefix.
//
// If knownSpeakers is non-empty, ONLY those names are accepted;
// this resolves the "Warning:" vs "Proog:" ambiguity using context
// instead of guessing. If knownSpeakers is empty, the permissive
// heuristic is used.
bool splitSpeakerLine(
    const std::string& line,
    std::size_t maxSpeakerLength,
    const std::set<std::string>& knownSpeakers,
    std::string& speakerOut,
    std::string& textOut)
{
    const std::size_t colon = line.find(':');

    if (colon == std::string::npos)
    {
        return false;
    }

    const std::string_view name =
        trim(std::string_view(line).substr(0, colon));

    if (!isPlausibleSpeaker(name, maxSpeakerLength))
    {
        return false;
    }

    // Require whitespace right after the colon (i.e. "Proog: text"),
    // which further separates dialogue from things like "12:34".
    if (colon + 1 >= line.size() ||
        std::isspace(static_cast<unsigned char>(line[colon + 1])) == 0)
    {
        return false;
    }

    // Allowlist gate: when known speakers are supplied, a plausible-looking
    // prefix that is NOT in the set (e.g. "Warning") is rejected.
    if (!knownSpeakers.empty() &&
        knownSpeakers.find(std::string(name)) == knownSpeakers.end())
    {
        return false;
    }

    speakerOut.assign(name);
    textOut.assign(trim(std::string_view(line).substr(colon + 1)));
    return true;
}

std::vector<std::string> splitLines(const std::string& text)
{
    std::vector<std::string> lines;
    std::string current;
    std::istringstream stream(text);

    while (std::getline(stream, current))
    {
        lines.push_back(current);
    }

    if (lines.empty())
    {
        lines.push_back(text);
    }

    return lines;
}

} // namespace

MetadataExtractor::MetadataExtractor(MetadataExtractorOptions options)
    : mOptions(std::move(options))
{
}

void MetadataExtractor::apply(CaptionCue& cue) const
{
    // 1. Language comes from external context, never inferred from text.
    if (!mOptions.default_language.empty() &&
        cue.metadata.language.empty())
    {
        cue.metadata.language = mOptions.default_language;
    }

    if (!mOptions.extract_speaker)
    {
        return;
    }

    const std::vector<std::string> lines = splitLines(cue.text);

    std::string firstSpeaker;
    bool allSameSpeaker = true;
    bool anySpeakerFound = false;

    std::vector<std::string> strippedLines;
    strippedLines.reserve(lines.size());

    for (const std::string& line : lines)
    {
        std::string speaker;
        std::string body;

        if (splitSpeakerLine(
                line,
                mOptions.max_speaker_name_length,
                mOptions.known_speakers,   // <-- pass the allowlist
                speaker,
                body))
        {
            if (!anySpeakerFound)
            {
                firstSpeaker = speaker;
                anySpeakerFound = true;
            }
            else if (speaker != firstSpeaker)
            {
                allSameSpeaker = false;
            }

            strippedLines.push_back(body);
        }
        else
        {
            // Line without a recognised prefix; keep it verbatim.
            allSameSpeaker = false;
            strippedLines.push_back(line);
        }
    }

    // Only touch the cue if we actually recognised a speaker.
    if (!anySpeakerFound)
    {
        return;
    }

    // Record the cue-level speaker as the first speaker seen.
    // Gender / voice are deliberately left as Unknown; they must be
    // resolved later from an explicit speaker -> voice map.
    cue.metadata.speaker_name = firstSpeaker;

    // Rebuild the text with the prefixes removed on every line.
    std::string rebuilt;
    for (std::size_t i = 0; i < strippedLines.size(); ++i)
    {
        if (i != 0)
        {
            rebuilt += '\n';
        }
        rebuilt += strippedLines[i];
    }

    cue.text = rebuilt;

    (void)allSameSpeaker;
}

} // namespace edgeai
