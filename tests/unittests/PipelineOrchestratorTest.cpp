#include <gtest/gtest.h>

#include "PipelineOrchestrator.h"

// AI-37-01: proves the hardcoded 2-node no-op pipeline actually executes end-to-end
// through the Orchestrator, in order, rather than just eyeballing log output.
TEST(PipelineOrchestrator, RunsHardcodedTwoNodeDummyPipelineInOrder)
{
    edgeai::PipelineOrchestrator orchestrator;
    const auto executed = orchestrator.runDummyPipeline();

    ASSERT_EQ(executed.size(), 2u);
    EXPECT_EQ(executed[0], "nodeA");
    EXPECT_EQ(executed[1], "nodeB");
}
