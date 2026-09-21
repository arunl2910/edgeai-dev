# rdke-edgeai

Out-of-process EdgeAI platform component (org.rdk.EdgeAI) for RDK.

- `engine/`      Thunder plugin + core (Capability Registry, Pipeline Orchestrator, Task-Node Registry, Model Manager, Scheduler)
- `task-nodes/`  Pluggable task-node implementations (caption-source, defrag, schedule)
- `hal/`         Neutral HAL contract (`edgeai_hal.h`) + vendor backend(s)
- `catalog/`     Demo content catalog (clear/licensed streams + caption metadata)

Architecture context: see `design/` in the `edgeai-bcm` workspace.

## Build

Standalone (host, core + task-node libs only, no Thunder plugin):

    cmake -B build -DEDGEAI_BUILD_THUNDER_PLUGIN=OFF
    cmake --build build

Full build (Thunder plugin + vendor HAL, requires the WPEFramework SDK and
vendor SDKs — done via the `meta-rdke-edgeai` Yocto layer):

    bitbake packagegroup-edgeai

## Tests (AI-37-01: dummy-pipeline wiring)

    cmake -B build -DEDGEAI_BUILD_THUNDER_PLUGIN=OFF -DEDGEAI_BUILD_TESTS=ON
    cmake --build build
    ./build/tests/unittests/edgeai_unittests

Uses gtest (fetched via `FetchContent` for local dev; the product build would use
meta-oe's `googletest` recipe instead, e.g. via ptest — not wired up yet). Covers:
the 5 core stub components construct + log init, and the hardcoded 2-node dummy
pipeline executes `nodeA -> nodeB` in order through the Orchestrator.
