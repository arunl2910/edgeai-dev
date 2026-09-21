#include <gtest/gtest.h>

#include <string>

#include "ManifestLoader.h"

namespace {
std::string samplePath(const std::string& name)
{
    return std::string(EDGEAI_SAMPLES_DIR) + "/" + name;
}
} // namespace

TEST(ManifestLoader, LoadsValidAudioTranslatePipelineTemplate)
{
    edgeai::ManifestLoader loader;
    std::string error;
    EXPECT_TRUE(loader.loadPipelineTemplate(samplePath("media.audio.translate.pipeline.json"), error)) << error;
    EXPECT_TRUE(error.empty());
}

TEST(ManifestLoader, LoadsValidTranslateManifest)
{
    edgeai::ManifestLoader loader;
    std::string error;
    EXPECT_TRUE(loader.loadModelManifest(samplePath("translate.en-es.manifest.json"), error)) << error;
}

TEST(ManifestLoader, LoadsValidTtsManifest)
{
    edgeai::ManifestLoader loader;
    std::string error;
    EXPECT_TRUE(loader.loadModelManifest(samplePath("tts.es.manifest.json"), error)) << error;
}

TEST(ManifestLoader, RejectsMalformedPipelineTemplateWithClearError)
{
    edgeai::ManifestLoader loader;
    std::string error;
    EXPECT_FALSE(loader.loadPipelineTemplate(samplePath("invalid.pipeline.json"), error));
    EXPECT_NE(error.find("nodes[1]"), std::string::npos) << "error was: " << error;
}
