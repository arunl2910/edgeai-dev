# SHARED (not STATIC per the task-node template) so the .srt parsing core can be
# swapped/updated independently of the nodes that link against it.
add_library(edgeai_node_caption_source SHARED
    src/CaptionSourceNode.cpp
    src/CueParser.cpp
    src/MetadataExtractor.cpp
    src/SrtBlockReader.cpp
    src/TimestampParser.cpp
    src/TimingLineParser.cpp
)
target_include_directories(edgeai_node_caption_source PUBLIC include)

