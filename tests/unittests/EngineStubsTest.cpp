#include <gtest/gtest.h>

#include "CapabilityRegistry.h"
#include "ModelManager.h"
#include "Scheduler.h"
#include "TaskNodeRegistry.h"

// AI-37-01: stub components only need to construct/log without crashing at this stage.
TEST(EngineStubs, AllCoreComponentsConstructAndLogInit)
{
    edgeai::CapabilityRegistry capabilityRegistry;
    edgeai::TaskNodeRegistry taskNodeRegistry;
    edgeai::ModelManager modelManager;
    edgeai::Scheduler scheduler;

    EXPECT_NO_THROW(capabilityRegistry.logInit());
    EXPECT_NO_THROW(taskNodeRegistry.logInit());
    EXPECT_NO_THROW(modelManager.logInit());
    EXPECT_NO_THROW(scheduler.logInit());
}
