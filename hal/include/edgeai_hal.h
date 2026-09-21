#ifndef EDGEAI_HAL_H
#define EDGEAI_HAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Neutral, in-process C ABI crossed by the Orchestrator to reach a vendor/SoC backend.
 * Never crossed directly by use-case-specific code (see design/01-platform-architecture.confluence). */

typedef struct edgeai_model edgeai_model;

typedef struct
{
    const char* name;
    uint32_t version;
} edgeai_capability;

typedef void (*edgeai_status_cb)(int status, void* user_data);

/* Query the capabilities the backend actually supports on this device. */
int edgeai_query_caps(edgeai_capability** caps, size_t* count);

/* Register a callback for asynchronous backend status/health events. */
int edgeai_register_status_cb(edgeai_status_cb cb, void* user_data);

/* Load a model artifact for a task onto a target execution unit (NPU/DSP/CPU) at a QoS class. */
int edgeai_model_load(const char* exec_model, const void* artifact, size_t nbytes,
                       int target_id, int qos, edgeai_model** model);

/* Run inference on a loaded model. */
int edgeai_infer(edgeai_model* model, const void* input, size_t input_bytes,
                  void* output, size_t* output_bytes);

#ifdef __cplusplus
}
#endif

#endif /* EDGEAI_HAL_H */
