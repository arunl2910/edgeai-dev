#pragma once

namespace edgeai {

// Derived view over declared capabilities (capability descriptor -> model manifest -> artifact).
// Populated via schema/loader manifests once AI-37-02 lands. Not implemented yet (stub).
class CapabilityRegistry
{
public:
    CapabilityRegistry();
    ~CapabilityRegistry();

    void logInit() const;
};

} // namespace edgeai
