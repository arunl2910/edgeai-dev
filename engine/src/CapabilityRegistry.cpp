#include "CapabilityRegistry.h"
#include "Logging.h"

namespace edgeai {

CapabilityRegistry::CapabilityRegistry() = default;
CapabilityRegistry::~CapabilityRegistry() = default;

void CapabilityRegistry::logInit() const
{
    logInfo("CapabilityRegistry", "initialized (stub, no capability data loaded)");
}

} // namespace edgeai
