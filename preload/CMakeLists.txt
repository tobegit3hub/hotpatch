
include_directories(
    ${INCLUDE_DIRECTORIES}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}/src
)

add_library(preload_hotpatch SHARED preload_hotpatch.cpp)
target_include_directories(preload_hotpatch PUBLIC "/usr/local/include/")
target_link_directories(preload_hotpatch PUBLIC "/usr/local/lib/")
target_link_libraries(preload_hotpatch hotpatch_server)
