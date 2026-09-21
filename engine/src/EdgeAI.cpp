#include "EdgeAI.h"

namespace WPEFramework {
namespace Plugin {

SERVICE_REGISTRATION(EdgeAI, 1, 0, 0);

EdgeAI::EdgeAI() = default;

const string EdgeAI::Initialize(PluginHost::IShell* service)
{
    ASSERT(service != nullptr);
    _service = service;
    _service->AddRef();

    // Spawns EdgeAIImplementation out-of-process (root.mode in EdgeAI.conf controls
    // whether Thunder actually forks a separate process vs. running in-process).
    _implementation = _service->Root<Exchange::IEdgeAI>(_connectionId, 2000, _T("EdgeAIImplementation"));
    if (_implementation == nullptr)
    {
        _service->Release();
        _service = nullptr;
        return _T("EdgeAI could not be instantiated (Root<IEdgeAI> failed)");
    }

    RegisterMethods();
    return {}; // empty string == success
}

void EdgeAI::Deinitialize(PluginHost::IShell* /*service*/)
{
    UnregisterMethods();

    if (_implementation != nullptr)
    {
        _implementation->Release();
        _implementation = nullptr;
    }
    if (_service != nullptr)
    {
        _service->Release();
        _service = nullptr;
    }
}

string EdgeAI::Information() const
{
    return {};
}

void EdgeAI::RegisterMethods()
{
    Register<JsonObject, JsonObject>(_T("ping"), &EdgeAI::Ping, this);
}

void EdgeAI::UnregisterMethods()
{
    Unregister(_T("ping"));
}

uint32_t EdgeAI::Ping(const JsonObject& /*parameters*/, JsonObject& response)
{
    if (_implementation == nullptr)
    {
        return Core::ERROR_UNAVAILABLE;
    }

    string status;
    const uint32_t result = _implementation->Ping(status);
    response["status"] = status;
    response["component"] = "org.rdk.EdgeAI";
    return result;
}

} // namespace Plugin
} // namespace WPEFramework
