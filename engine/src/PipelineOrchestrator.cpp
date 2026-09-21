#include "PipelineOrchestrator.h"
#include "Logging.h"

namespace edgeai {

PipelineOrchestrator::PipelineOrchestrator() = default;
PipelineOrchestrator::~PipelineOrchestrator() = default;

void PipelineOrchestrator::logInit() const
{
    logInfo("PipelineOrchestrator", "initialized (stub)");
}

std::vector<std::string> PipelineOrchestrator::runDummyPipeline() const
{
    logInfo("PipelineOrchestrator", "RESOLVE: dummy-pipeline (nodeA -> nodeB)");

    std::vector<std::string> executed;
    for (const std::string& nodeId : {std::string("nodeA"), std::string("nodeB")})
    {
        logInfo("PipelineOrchestrator", "EXECUTE: " + nodeId + " (no-op)");
        executed.push_back(nodeId);
    }

    logInfo("PipelineOrchestrator", "dummy pipeline complete");
    return executed;
}

} // namespace edgeai
