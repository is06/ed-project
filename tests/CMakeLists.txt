set(EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE})

find_package(Catch2 REQUIRED)

file(
    GLOB_RECURSE
    test_source_files
    *.cpp
)

add_executable(
    ${CMAKE_PROJECT_NAME}_test
    ${test_source_files}
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}_test
    PRIVATE Catch2::Catch2
    ed_lib
)

include(CTest)
include(Catch)
catch_discover_tests(${CMAKE_PROJECT_NAME}_test)
