cmake_minimum_required(VERSION 3.0)

project(hotpatch)

add_compile_options(-std=c++14)

# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC"  )

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/thirdparty/include)
link_directories(${PROJECT_SOURCE_DIR}/thirdparty/lib)

include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory("src")
add_subdirectory("examples")
add_subdirectory("benchmark")
add_subdirectory("preload")

enable_testing()