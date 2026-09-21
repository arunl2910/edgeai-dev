#pragma once

namespace edgeai {

// Open, pluggable registry of task-node operator types (translate, tts, caption-source,
// defrag, schedule, ...). Not implemented yet (stub).
class TaskNodeRegistry
{
public:
    TaskNodeRegistry();
    ~TaskNodeRegistry();

    void logInit() const;
};

} // namespace edgeai
