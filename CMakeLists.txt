cmake_minimum_required(VERSION 3.0)

project(ed)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2 -Wall -pedantic")

find_package(Irrlicht REQUIRED)

file(
    GLOB_RECURSE
    source_files
    src/*.cpp
)

add_executable(
    ed
    ${source_files}
)

target_link_libraries(
    ed
    ${IRRLICHT_LIBRARY}
)
