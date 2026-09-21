add_library(edgeai_core STATIC
    src/CapabilityRegistry.cpp
    src/PipelineOrchestrator.cpp
    src/TaskNodeRegistry.cpp
    src/ModelManager.cpp
    src/Scheduler.cpp
)
target_include_directories(edgeai_core PUBLIC include)

if(EDGEAI_BUILD_THUNDER_PLUGIN)
    find_package(WPEFramework REQUIRED)
    add_library(EdgeAI SHARED
        src/EdgeAI.cpp
        src/EdgeAIImplementation.cpp
    )
    target_link_libraries(EdgeAI PRIVATE edgeai_core WPEFramework::Core WPEFramework::Plugins WPEFramework::COM)
endif()
