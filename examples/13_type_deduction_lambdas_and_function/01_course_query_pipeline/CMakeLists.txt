cmake_minimum_required(VERSION 3.16)
project(course_query_pipeline LANGUAGES CXX)

add_executable(course_query_pipeline course_query_pipeline.cpp)
target_compile_features(course_query_pipeline PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        course_query_pipeline
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS deduction structured generic optional capture)
    add_test(
        NAME query_${case_name}
        COMMAND course_query_pipeline --case ${case_name}
    )
endforeach()

add_test(NAME query_all COMMAND course_query_pipeline)
add_test(
    NAME query_invalid_case
    COMMAND course_query_pipeline --case missing
)
set_tests_properties(query_invalid_case PROPERTIES WILL_FAIL TRUE)
