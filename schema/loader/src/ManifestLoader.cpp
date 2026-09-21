#include "ManifestLoader.h"

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

namespace edgeai {

namespace {

using nlohmann::json;

bool readJsonFile(const std::string& path, json& out, std::string& errorOut)
{
    std::ifstream in(path);
    if (!in.is_open())
    {
        errorOut = "cannot open file: " + path;
        return false;
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    try
    {
        out = json::parse(buffer.str());
    }
    catch (const json::parse_error& e)
    {
        errorOut = std::string("JSON parse error in ") + path + ": " + e.what();
        return false;
    }
    return true;
}

bool requireString(const json& obj, const std::string& field, const std::string& context, std::string& errorOut)
{
    if (!obj.contains(field))
    {
        errorOut = context + ": missing required field '" + field + "'";
        return false;
    }
    if (!obj.at(field).is_string())
    {
        errorOut = context + ": field '" + field + "' must be a string";
        return false;
    }
    return true;
}

} // namespace

bool ManifestLoader::loadPipelineTemplate(const std::string& path, std::string& errorOut)
{
    json doc;
    if (!readJsonFile(path, doc, errorOut))
        return false;

    if (!requireString(doc, "pipeline", "pipeline template", errorOut))
        return false;

    if (!doc.contains("nodes") || !doc.at("nodes").is_array())
    {
        errorOut = "pipeline template: missing required array field 'nodes'";
        return false;
    }

    const auto& nodes = doc.at("nodes");
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        const auto& node = nodes[i];
        const std::string context = "pipeline template: nodes[" + std::to_string(i) + "]";
        if (!node.is_object())
        {
            errorOut = context + " must be an object";
            return false;
        }
        if (!requireString(node, "id", context, errorOut))
            return false;
        if (!requireString(node, "type", context, errorOut))
            return false;
    }

    if (!doc.contains("edges") || !doc.at("edges").is_array())
    {
        errorOut = "pipeline template: missing required array field 'edges'";
        return false;
    }

    const auto& edges = doc.at("edges");
    for (size_t i = 0; i < edges.size(); ++i)
    {
        const auto& edge = edges[i];
        const std::string context = "pipeline template: edges[" + std::to_string(i) + "]";
        if (!edge.is_object())
        {
            errorOut = context + " must be an object";
            return false;
        }
        if (!requireString(edge, "from", context, errorOut))
            return false;
        if (!requireString(edge, "to", context, errorOut))
            return false;
    }

    return true;
}

bool ManifestLoader::loadModelManifest(const std::string& path, std::string& errorOut)
{
    json doc;
    if (!readJsonFile(path, doc, errorOut))
        return false;

    if (!requireString(doc, "task", "model manifest", errorOut))
        return false;

    if (!doc.contains("artifact") || !doc.at("artifact").is_object())
    {
        errorOut = "model manifest: missing required object field 'artifact'";
        return false;
    }

    const auto& artifact = doc.at("artifact");
    if (!requireString(artifact, "path", "model manifest: artifact", errorOut))
        return false;
    if (!requireString(artifact, "format", "model manifest: artifact", errorOut))
        return false;

    return true;
}

} // namespace edgeai
