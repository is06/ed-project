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

add_test(
    NAME ${CMAKE_PROJECT_NAME}_test
    COMMAND ${CMAKE_PROJECT_NAME}_test
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}_test
    PRIVATE Catch2::Catch2
    ed_lib
)
