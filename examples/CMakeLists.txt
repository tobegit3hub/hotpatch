
include_directories(
    ${INCLUDE_DIRECTORIES}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}/src
)

add_library(add_func_patch1 SHARED add_func.cpp before_add_func.cpp after_add_func.cpp)

add_executable(simple_server simple_server.cpp)
target_link_libraries(simple_server hotpatch_server)

add_executable(prebuilt_server prebuilt_server.cpp)
target_link_libraries(prebuilt_server hotpatch_server)
