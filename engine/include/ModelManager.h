#pragma once

namespace edgeai {

// Owns model manifests and resolves them to on-disk/on-device artifacts. Not implemented yet (stub).
class ModelManager
{
public:
    ModelManager();
    ~ModelManager();

    void logInit() const;
};

} // namespace edgeai
