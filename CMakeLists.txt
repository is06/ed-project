cmake_minimum_required(VERSION 3.5)

project(ed LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2 -Wall -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pedantic")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

find_package(Irrlicht REQUIRED)
include_directories(${IRRLICHT_INCLUDE_DIR})

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
