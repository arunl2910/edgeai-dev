#pragma once

namespace edgeai {

// Hardware/backend arbitration (NPU/DSP/CPU), separate from the Orchestrator's dataflow-only
// role. Not implemented yet (stub).
class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    void logInit() const;
};

} // namespace edgeai
