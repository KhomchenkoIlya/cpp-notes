cmake_minimum_required(VERSION 3.16)
project(course_catalog_pipeline LANGUAGES CXX)

add_executable(course_catalog_pipeline course_catalog_pipeline.cpp)
target_compile_features(course_catalog_pipeline PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        course_catalog_pipeline
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS lookup filter transform summary pipeline)
    add_test(
        NAME catalog_${case_name}
        COMMAND course_catalog_pipeline --case ${case_name}
    )
endforeach()

add_test(NAME catalog_all COMMAND course_catalog_pipeline)
add_test(
    NAME catalog_invalid_case
    COMMAND course_catalog_pipeline --case missing
)
set_tests_properties(catalog_invalid_case PROPERTIES WILL_FAIL TRUE)
