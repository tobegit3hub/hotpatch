
add_library(hotpatch hotpatch_server.cpp)

add_executable(simple_server simple_server.cpp)

target_include_directories(simple_server PUBLIC "/usr/local/include/")
target_link_directories(simple_server PUBLIC "/usr/local/lib/")
target_link_libraries(simple_server hotpatch gflags pthread)
