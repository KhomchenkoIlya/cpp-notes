cmake_minimum_required(VERSION 3.16)
project(error_model_lab LANGUAGES CXX)

add_executable(error_model_lab error_model_lab.cpp)
target_compile_features(error_model_lab PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(error_model_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
foreach(case status-success status-failure exception-success exception-failure
             unwind rethrow cleanup-normal cleanup-exception)
    add_test(NAME ${case} COMMAND error_model_lab ${case} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
set_tests_properties(status-failure exception-failure PROPERTIES WILL_FAIL TRUE)
set_tests_properties(unwind PROPERTIES
    PASS_REGULAR_EXPRESSION "- low.*- middle.*- top")
set_tests_properties(rethrow PROPERTIES
    PASS_REGULAR_EXPRESSION "middle context: deep failure.*top boundary caught: deep failure")
