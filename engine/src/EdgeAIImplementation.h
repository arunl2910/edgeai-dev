#pragma once

// Requires the WPEFramework/Thunder SDK headers to compile. Only built when
// EDGEAI_BUILD_THUNDER_PLUGIN=ON, via the meta-rdke-edgeai Yocto recipe.
#include <plugins/plugins.h>

#include "CapabilityRegistry.h"
#include "IEdgeAI.h"
#include "ModelManager.h"
#include "PipelineOrchestrator.h"
#include "Scheduler.h"
#include "TaskNodeRegistry.h"

namespace WPEFramework {
namespace Plugin {

// Out-of-process object (spawned via EdgeAI::Initialize()'s Root<IEdgeAI>() call).
// This is where the 5 core components actually live -- verified real convention,
// modeled on entservices-displayinfo's DisplayInfo -> DisplayInfoImplementation split.
class EdgeAIImplementation : public Exchange::IEdgeAI
{
public:
    EdgeAIImplementation(const EdgeAIImplementation&) = delete;
    EdgeAIImplementation& operator=(const EdgeAIImplementation&) = delete;

    EdgeAIImplementation();
    ~EdgeAIImplementation() override = default;

    BEGIN_INTERFACE_MAP(EdgeAIImplementation)
    INTERFACE_ENTRY(Exchange::IEdgeAI)
    END_INTERFACE_MAP

    uint32_t Ping(string& response) const override;

private:
    edgeai::CapabilityRegistry _capabilityRegistry;
    edgeai::PipelineOrchestrator _pipelineOrchestrator;
    edgeai::TaskNodeRegistry _taskNodeRegistry;
    edgeai::ModelManager _modelManager;
    edgeai::Scheduler _scheduler;
};

} // namespace Plugin
} // namespace WPEFramework
