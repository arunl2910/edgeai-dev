#pragma once

// Requires the WPEFramework/Thunder SDK headers to compile. Only built when
// EDGEAI_BUILD_THUNDER_PLUGIN=ON, via the meta-rdke-edgeai Yocto recipe.
#include <plugins/plugins.h>

#include "CapabilityRegistry.h"
#include "ModelManager.h"
#include "PipelineOrchestrator.h"
#include "Scheduler.h"
#include "TaskNodeRegistry.h"

namespace WPEFramework {
namespace Plugin {

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
    uint32_t Ping(const JsonObject& parameters, JsonObject& response);

    edgeai::CapabilityRegistry _capabilityRegistry;
    edgeai::PipelineOrchestrator _pipelineOrchestrator;
    edgeai::TaskNodeRegistry _taskNodeRegistry;
    edgeai::ModelManager _modelManager;
    edgeai::Scheduler _scheduler;
};

} // namespace Plugin
} // namespace WPEFramework
