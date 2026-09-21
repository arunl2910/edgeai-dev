#include "EdgeAIImplementation.h"

namespace WPEFramework {
namespace Plugin {

SERVICE_REGISTRATION(EdgeAIImplementation, 1, 0, 0);

EdgeAIImplementation::EdgeAIImplementation()
{
    _capabilityRegistry.logInit();
    _pipelineOrchestrator.logInit();
    _taskNodeRegistry.logInit();
    _modelManager.logInit();
    _scheduler.logInit();

    // Proves Orchestrator wiring end-to-end with a hardcoded 2-node no-op pipeline (AI-37-01).
    _pipelineOrchestrator.runDummyPipeline();
}

uint32_t EdgeAIImplementation::Ping(string& response) const
{
    response = "ok";
    return Core::ERROR_NONE;
}

} // namespace Plugin
} // namespace WPEFramework
