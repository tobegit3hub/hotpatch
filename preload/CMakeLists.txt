
include_directories(
    ${INCLUDE_DIRECTORIES}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}/src
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_library(preload_hotpatch_linux SHARED preload_hotpatch_linux.cpp)
    target_include_directories(preload_hotpatch_linux PUBLIC "/usr/local/include/")
    target_link_directories(preload_hotpatch_linux PUBLIC "/usr/local/lib/")
    target_link_libraries(preload_hotpatch_linux hotpatch_server)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_library(preload_hotpatch_mac SHARED preload_hotpatch_mac.cpp)
    target_include_directories(preload_hotpatch_mac PUBLIC "/usr/local/include/")
    target_link_directories(preload_hotpatch_mac PUBLIC "/usr/local/lib/")
    target_link_libraries(preload_hotpatch_mac hotpatch_server)
endif ()