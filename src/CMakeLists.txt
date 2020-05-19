
set(HOTPATCH_HEADERS hotpatch_server.h hotpatch_command.h)
set(HOTPATCH_SOURCES hotpatch_command.cpp hotpatch_server.cpp)

add_library(hotpatch_server ${HOTPATCH_SOURCES})
target_include_directories(hotpatch_server PUBLIC "/usr/local/include/")
target_link_directories(hotpatch_server PUBLIC "/usr/local/lib/")
target_link_libraries(hotpatch_server gflags glog pthread dl subhook)

add_executable(HotpatchCommandTest hotpatch_command_test.cpp)
target_link_libraries(HotpatchCommandTest ${GTEST_BOTH_LIBRARIES} pthread)
add_test(Test HotpatchCommandTest)
enable_testing()

#if(APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT SUBHOOK_FORCE_32BIT)
#    set_property(TARGET subhook_cxx_test_exe2 APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-no_pie")
#endif()

install(TARGETS hotpatch_server
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(FILES ${HOTPATCH_HEADERS} DESTINATION include)
