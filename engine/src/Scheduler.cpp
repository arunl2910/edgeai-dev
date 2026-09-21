#include "Scheduler.h"
#include "Logging.h"

namespace edgeai {

Scheduler::Scheduler() = default;
Scheduler::~Scheduler() = default;

void Scheduler::logInit() const
{
    logInfo("Scheduler", "initialized (stub, no backends registered)");
}

} // namespace edgeai
