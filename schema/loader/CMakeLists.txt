add_library(edgeai_schema_loader STATIC
    src/ManifestLoader.cpp
)
target_include_directories(edgeai_schema_loader PUBLIC include)
target_link_libraries(edgeai_schema_loader PUBLIC nlohmann_json::nlohmann_json)
