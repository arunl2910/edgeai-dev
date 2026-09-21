#include "edgeai_hal.h"

/* Broadcom/bstorm backend implementation of edgeai_hal.h. Not implemented yet (stub, G3). */

extern "C" {

int edgeai_query_caps(edgeai_capability** caps, size_t* count)
{
    *caps = nullptr;
    *count = 0;
    return 0;
}

int edgeai_register_status_cb(edgeai_status_cb /*cb*/, void* /*user_data*/)
{
    return 0;
}

int edgeai_model_load(const char* /*exec_model*/, const void* /*artifact*/, size_t /*nbytes*/,
                       int /*target_id*/, int /*qos*/, edgeai_model** model)
{
    *model = nullptr;
    return -1; /* not implemented */
}

int edgeai_infer(edgeai_model* /*model*/, const void* /*input*/, size_t /*input_bytes*/,
                  void* /*output*/, size_t* output_bytes)
{
    *output_bytes = 0;
    return -1; /* not implemented */
}

} /* extern "C" */
