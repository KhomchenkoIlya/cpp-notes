cmake_minimum_required(VERSION 3.16)

project(control_flow_lab LANGUAGES CXX)

add_executable(branching cases/branching.cpp)
add_executable(loop_boundary cases/loop_boundary.cpp)
add_executable(short_circuit cases/short_circuit.cpp)
add_executable(logic_error cases/logic_error.cpp)

foreach(target branching loop_boundary short_circuit logic_error)
    target_compile_features(${target} PRIVATE cxx_std_17)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

enable_testing()
add_test(NAME branching_three_paths COMMAND branching)
set_tests_properties(branching_three_paths PROPERTIES PASS_REGULAR_EXPRESSION "low.*middle.*high")
add_test(NAME loop_boundary COMMAND loop_boundary)
set_tests_properties(loop_boundary PROPERTIES PASS_REGULAR_EXPRESSION "count=5 sum=15")
add_test(NAME short_circuit COMMAND short_circuit)
set_tests_properties(short_circuit PROPERTIES PASS_REGULAR_EXPRESSION "guarded=false checks=1")
