add_library(edgeai_node_defrag STATIC
    src/DefragNode.cpp
)
target_include_directories(edgeai_node_defrag PUBLIC include)
target_link_libraries(edgeai_node_defrag PUBLIC edgeai_node_caption_source)
