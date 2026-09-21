#pragma once

// Requires the WPEFramework/Thunder SDK headers to compile. Only built when
// EDGEAI_BUILD_THUNDER_PLUGIN=ON, via the meta-rdke-edgeai Yocto recipe.
#include <plugins/plugins.h>

#include "IEdgeAI.h"

namespace WPEFramework {
namespace Plugin {

// Thin in-process plugin: JSON-RPC glue only. The 5 core components live in
// EdgeAIImplementation, spawned out-of-process via Root<IEdgeAI>() below (the
// actual "Thunder split-plugin shape", verified against entservices-displayinfo).
class EdgeAI : public PluginHost::IPlugin, public PluginHost::JSONRPC
{
public:
    EdgeAI(const EdgeAI&) = delete;
    EdgeAI& operator=(const EdgeAI&) = delete;

    EdgeAI();
    ~EdgeAI() override = default;

    BEGIN_INTERFACE_MAP(EdgeAI)
    INTERFACE_ENTRY(PluginHost::IPlugin)
    INTERFACE_ENTRY(PluginHost::IDispatcher)
    END_INTERFACE_MAP

    // IPlugin overrides
    const string Initialize(PluginHost::IShell* service) override;
    void Deinitialize(PluginHost::IShell* service) override;
    string Information() const override;

private:
    void RegisterMethods();
    void UnregisterMethods();
    uint32_t Ping(const JsonObject& parameters, JsonObject& response);

    PluginHost::IShell* _service{nullptr};
    uint32_t _connectionId{0};
    Exchange::IEdgeAI* _implementation{nullptr};
};

} // namespace Plugin
} // namespace WPEFramework
