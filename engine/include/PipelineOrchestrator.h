#pragma once

#include <string>
#include <vector>

namespace edgeai {

// Executes a pipeline DAG: RESOLVE (validate spec, bind capability->model->artifact,
// instantiate nodes+manifest pre/post) then EXECUTE (topological order, pre->Scheduler->
// HAL.infer->post, move envelopes). See design/01-platform-architecture.confluence.
class PipelineOrchestrator
{
public:
    PipelineOrchestrator();
    ~PipelineOrchestrator();

    void logInit() const;

    // Hardcoded 2-node no-op pipeline proving RESOLVE->EXECUTE wiring (AI-37-01).
    // Returns the executed node ids in order, so the wiring can be asserted in a test.
    std::vector<std::string> runDummyPipeline() const;
};

} // namespace edgeai
