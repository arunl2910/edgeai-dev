#include "EdgeAI.h"

namespace WPEFramework {
namespace Plugin {

SERVICE_REGISTRATION(EdgeAI, 1, 0);

EdgeAI::EdgeAI() = default;

const string EdgeAI::Initialize(PluginHost::IShell* /*service*/)
{
    _capabilityRegistry.logInit();
    _pipelineOrchestrator.logInit();
    _taskNodeRegistry.logInit();
    _modelManager.logInit();
    _scheduler.logInit();

    RegisterMethods();

    // Proves Orchestrator wiring end-to-end with a hardcoded 2-node no-op pipeline (AI-37-01).
    _pipelineOrchestrator.runDummyPipeline();

    return {}; // empty string == success
}

void EdgeAI::Deinitialize(PluginHost::IShell* /*service*/)
{
    Unregister(_T("ping"));
}

string EdgeAI::Information() const
{
    return {};
}

void EdgeAI::RegisterMethods()
{
    Register<JsonObject, JsonObject>(_T("ping"), &EdgeAI::Ping, this);
}

uint32_t EdgeAI::Ping(const JsonObject& /*parameters*/, JsonObject& response)
{
    response["status"] = "ok";
    response["component"] = "org.rdk.EdgeAI";
    return Core::ERROR_NONE;
}

} // namespace Plugin
} // namespace WPEFramework
