cmake_minimum_required(VERSION 3.0)

project(hotpatch)

add_compile_options(-std=c++14)

add_subdirectory("src")
add_subdirectory("examples")
add_subdirectory("benchmark")