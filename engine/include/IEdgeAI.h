#pragma once

// TODO: once integrated with the real entservices-interfaces build, this should live there
// (as interfaces/IEdgeAI.h) and get a properly reserved ID_EDGEAI value from the shared
// interface ID registry, matching how Exchange::IConnectionProperties etc. are defined.
#include <com/com.h>
#include <core/core.h>

namespace WPEFramework {
namespace Exchange {

// Minimal COM-RPC interface crossed by EdgeAI (in-process plugin) to reach
// EdgeAIImplementation (out-of-process object), via IShell::Root<IEdgeAI>().
struct IEdgeAI : virtual public Core::IUnknown
{
    enum { ID = ID_EDGEAI_INTERFACE_BASE }; // placeholder, not a reserved ID yet

    ~IEdgeAI() override = default;

    // Health check proving the out-of-process object is alive and all 5 core
    // components (Capability Registry, Orchestrator, Task-Node Registry,
    // Model Manager, Scheduler) initialized (AI-37-01).
    virtual uint32_t Ping(string& response /* @out */) const = 0;
};

} // namespace Exchange
} // namespace WPEFramework
