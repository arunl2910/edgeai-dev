#include "TaskNodeRegistry.h"
#include "Logging.h"

namespace edgeai {

TaskNodeRegistry::TaskNodeRegistry() = default;
TaskNodeRegistry::~TaskNodeRegistry() = default;

void TaskNodeRegistry::logInit() const
{
    logInfo("TaskNodeRegistry", "initialized (stub, no node types registered)");
}

} // namespace edgeai
