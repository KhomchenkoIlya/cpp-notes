cmake_minimum_required(VERSION 3.16)
project(inheritance_vs_composition_project_lab LANGUAGES CXX)

add_executable(project_lab project_lab.cpp)
target_compile_features(project_lab PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(project_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME project_designs_pass
    COMMAND project_lab --root ${CMAKE_CURRENT_LIST_DIR}/../../..)
add_test(NAME invalid_root_rejected
    COMMAND project_lab --root ${CMAKE_CURRENT_BINARY_DIR}/missing-root)
set_tests_properties(invalid_root_rejected PROPERTIES WILL_FAIL TRUE)
