#include "ModelManager.h"
#include "Logging.h"

namespace edgeai {

ModelManager::ModelManager() = default;
ModelManager::~ModelManager() = default;

void ModelManager::logInit() const
{
    logInfo("ModelManager", "initialized (stub, no manifests loaded)");
}

} // namespace edgeai
