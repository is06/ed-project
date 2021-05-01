cmake_minimum_required(VERSION 3.10)

project(ed)

include(CTest)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
