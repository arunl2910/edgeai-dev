add_executable(edgeai_unittests
    PipelineOrchestratorTest.cpp
    EngineStubsTest.cpp
)

target_link_libraries(edgeai_unittests PRIVATE
    edgeai_core
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(edgeai_unittests)
