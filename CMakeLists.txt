cmake_minimum_required(VERSION 3.10)
project(rdke-edgeai LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(EDGEAI_BUILD_ENGINE "Build the core registries + Thunder plugin" ON)
option(EDGEAI_BUILD_THUNDER_PLUGIN "Build the org.rdk.EdgeAI Thunder plugin (requires WPEFramework SDK)" OFF)
option(EDGEAI_BUILD_TASK_NODES "Build task-node plugins" ON)
option(EDGEAI_BUILD_HAL_BSTORM "Build the bstorm HAL backend (requires vendor SDK)" OFF)
option(EDGEAI_BUILD_TESTS "Build gtest unit tests" OFF)

if(EDGEAI_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
endif()

if(EDGEAI_BUILD_ENGINE)
    add_subdirectory(engine)
endif()

if(EDGEAI_BUILD_TASK_NODES)
    add_subdirectory(task-nodes)
endif()

if(EDGEAI_BUILD_HAL_BSTORM)
    add_subdirectory(hal/bstorm)
endif()

if(EDGEAI_BUILD_TESTS)
    add_subdirectory(tests/unittests)
endif()
