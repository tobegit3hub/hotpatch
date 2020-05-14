
add_library(hotpatch_command hotpatch_command.cpp)

add_library(hotpatch_server hotpatch_server.cpp)
target_include_directories(hotpatch_server PUBLIC "/usr/local/include/")
target_link_directories(hotpatch_server PUBLIC "/usr/local/lib/")
target_link_libraries(hotpatch_server hotpatch_command gflags glog pthread)

add_executable(simple_server simple_server.cpp)
target_link_libraries(simple_server hotpatch_server)
