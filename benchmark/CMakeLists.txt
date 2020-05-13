

include_directories(
    ${INCLUDE_DIRECTORIES}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}/src
)

add_executable(benchmark_hotpatch benchmark_hotpatch.cpp)
target_link_libraries(benchmark_hotpatch hotpatch)