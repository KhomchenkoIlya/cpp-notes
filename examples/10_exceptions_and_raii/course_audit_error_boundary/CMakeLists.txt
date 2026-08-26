cmake_minimum_required(VERSION 3.16)
project(course_audit_error_boundary LANGUAGES CXX)

add_executable(course_audit_error_boundary course_audit_error_boundary.cpp)
target_compile_features(course_audit_error_boundary PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(course_audit_error_boundary PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME audit_exception_real_course COMMAND course_audit_error_boundary
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --transport exception)
add_test(NAME audit_status_real_course COMMAND course_audit_error_boundary
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --transport status)
add_test(NAME completed_course_rejects_incomplete_expectation
    COMMAND course_audit_error_boundary
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --transport exception --expect-incomplete)
set_tests_properties(completed_course_rejects_incomplete_expectation
    PROPERTIES WILL_FAIL TRUE)
add_test(NAME audit_exception_io_failure COMMAND course_audit_error_boundary
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --transport exception --simulate-io)
set_tests_properties(audit_exception_io_failure PROPERTIES WILL_FAIL TRUE)
add_test(NAME audit_status_io_failure COMMAND course_audit_error_boundary
    --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --transport status --simulate-io)
set_tests_properties(audit_status_io_failure PROPERTIES WILL_FAIL TRUE)
add_test(NAME audit_exception_missing_fixture COMMAND course_audit_error_boundary
    --root ${CMAKE_CURRENT_BINARY_DIR}/missing-course --transport exception)
set_tests_properties(audit_exception_missing_fixture PROPERTIES WILL_FAIL TRUE)
add_test(NAME audit_status_missing_fixture COMMAND course_audit_error_boundary
    --root ${CMAKE_CURRENT_BINARY_DIR}/missing-course --transport status)
set_tests_properties(audit_status_missing_fixture PROPERTIES WILL_FAIL TRUE)
