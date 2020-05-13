
add_library(hotpatch hotpatch_server.cpp)
target_include_directories(hotpatch PUBLIC "/usr/local/include/")
target_link_directories(hotpatch PUBLIC "/usr/local/lib/")
target_link_libraries(hotpatch gflags glog pthread)

add_executable(simple_server simple_server.cpp)
target_link_libraries(simple_server hotpatch)
