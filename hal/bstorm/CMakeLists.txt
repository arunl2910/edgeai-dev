add_library(edgeai_hal_bstorm SHARED
    src/edgeai_hal_bstorm.cpp
)
target_include_directories(edgeai_hal_bstorm PUBLIC ${CMAKE_SOURCE_DIR}/hal/include)
