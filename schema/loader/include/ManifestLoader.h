#pragma once

#include <string>

namespace edgeai {

// Loads + validates pipeline templates and model manifests against the JSON schemas in
// schema/*.schema.json. Validation is intentionally basic (required fields + types, not
// full JSON Schema draft-07 compliance) per AI-37-02's acceptance criteria.
class ManifestLoader
{
public:
    // Returns true on success. On failure returns false and errorOut carries a specific,
    // human-readable reason (which field/node was missing or the wrong type).
    bool loadPipelineTemplate(const std::string& path, std::string& errorOut);
    bool loadModelManifest(const std::string& path, std::string& errorOut);
};

} // namespace edgeai
