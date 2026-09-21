add_executable(edgeai_unittests
    PipelineOrchestratorTest.cpp
    EngineStubsTest.cpp
    ManifestLoaderTest.cpp
)

target_link_libraries(edgeai_unittests PRIVATE
    edgeai_core
    edgeai_schema_loader
    GTest::gtest
    GTest::gtest_main
)

target_compile_definitions(edgeai_unittests PRIVATE
    EDGEAI_SAMPLES_DIR="${CMAKE_SOURCE_DIR}/schema/samples"
)

include(GoogleTest)
gtest_discover_tests(edgeai_unittests)
