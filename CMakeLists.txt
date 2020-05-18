cmake_minimum_required(VERSION 3.0)

project(hotpatch)

add_compile_options(-std=c++14)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/thirdparty/include)
link_directories(${PROJECT_SOURCE_DIR}/thirdparty/lib)

include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory("src")
add_subdirectory("examples")
add_subdirectory("benchmark")


enable_testing()