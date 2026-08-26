cmake_minimum_required(VERSION 3.16)
project(move_ownership_pipeline LANGUAGES CXX)

add_executable(move_ownership_pipeline move_ownership_pipeline.cpp)
target_compile_features(move_ownership_pipeline PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        move_ownership_pipeline
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS ctor assign category forward elision pipeline)
    add_test(
        NAME move_${case_name}
        COMMAND move_ownership_pipeline --case ${case_name}
    )
endforeach()

add_test(NAME move_all COMMAND move_ownership_pipeline)
add_test(
    NAME move_invalid_case
    COMMAND move_ownership_pipeline --case missing
)
set_tests_properties(move_invalid_case PROPERTIES WILL_FAIL TRUE)
