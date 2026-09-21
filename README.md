# rdke-edgeai

Out-of-process EdgeAI platform component (org.rdk.EdgeAI) for RDK.

- `engine/`      Thunder plugin + core (Capability Registry, Pipeline Orchestrator, Task-Node Registry, Model Manager, Scheduler)
- `schema/`      Capability descriptor / pipeline template / model manifest JSON schemas + loader
- `task-nodes/`  Pluggable task-node implementations (caption-source, defrag, schedule)
- `hal/`         Neutral HAL contract (`edgeai_hal.h`) + vendor backend(s)
- `catalog/`     Demo content catalog (clear/licensed streams + caption metadata)

Architecture context: see `design/` in the `edgeai-bcm` workspace.

## Build

Standalone (host, core + task-node + schema-loader libs, no Thunder plugin):

    cmake -B build -DEDGEAI_BUILD_THUNDER_PLUGIN=OFF
    cmake --build build

`schema/loader` depends on `nlohmann/json`. `find_package(nlohmann_json)` is tried
first (this is what happens in the real Yocto build, via the `nlohmann-json`
DEPENDS in `meta-rdke-edgeai`); if it's not found on the host, CMake fetches it
automatically via `FetchContent` for local dev convenience.

Full build (Thunder plugin + vendor HAL, requires the WPEFramework SDK and
vendor SDKs — done via the `meta-rdke-edgeai` Yocto layer):

    bitbake packagegroup-edgeai

## Tests (AI-37-01 dummy-pipeline wiring + AI-37-02 manifest loader)

    cmake -B build -DEDGEAI_BUILD_THUNDER_PLUGIN=OFF -DEDGEAI_BUILD_TESTS=ON
    cmake --build build
    ./build/tests/unittests/edgeai_unittests

Uses gtest (fetched via `FetchContent` for local dev; the product build would use
meta-oe's `googletest` recipe instead, e.g. via ptest — not wired up yet).
Covers: the 5 core stub components construct + log init; the hardcoded 2-node
dummy pipeline executes `nodeA -> nodeB` in order; the manifest loader accepts
the sample `media.audio.translate` pipeline template and both dummy manifests,
and rejects a malformed template with a specific field-level error.
