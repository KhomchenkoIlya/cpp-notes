cmake_minimum_required(VERSION 3.16)
project(course_audit_patterns LANGUAGES CXX)

add_executable(course_audit_patterns course_audit_patterns.cpp)
target_compile_features(course_audit_patterns PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(course_audit_patterns PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME structural_audit COMMAND course_audit_patterns
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --policy structural)
add_test(NAME strict_audit COMMAND course_audit_patterns
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --policy strict)
add_test(NAME extension_policy COMMAND course_audit_patterns
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --policy lecture-only)
add_test(NAME invalid_policy_rejected COMMAND course_audit_patterns
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --policy missing)
set_tests_properties(invalid_policy_rejected PROPERTIES WILL_FAIL TRUE)
