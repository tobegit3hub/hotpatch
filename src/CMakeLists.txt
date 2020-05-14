
add_library(hotpatch_command hotpatch_command.cpp)
target_include_directories(hotpatch_command PUBLIC "/usr/local/include/")
target_link_directories(hotpatch_command PUBLIC "/usr/local/lib/")
target_link_libraries(hotpatch_command gflags glog pthread)

add_library(hotpatch_server hotpatch_server.cpp)
target_include_directories(hotpatch_server PUBLIC "/usr/local/include/")
target_link_directories(hotpatch_server PUBLIC "/usr/local/lib/")
target_link_libraries(hotpatch_server hotpatch_command gflags glog pthread)

add_executable(HotpatchCommandTest hotpatch_command_test.cpp)
target_link_libraries(HotpatchCommandTest ${GTEST_BOTH_LIBRARIES})
add_test(Test HotpatchCommandTest)
enable_testing()