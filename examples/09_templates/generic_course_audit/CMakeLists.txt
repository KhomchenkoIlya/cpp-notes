cmake_minimum_required(VERSION 3.16)
project(generic_course_audit LANGUAGES CXX)

add_executable(generic_course_audit generic_course_audit.cpp)
target_compile_features(generic_course_audit PRIVATE cxx_std_17)
target_include_directories(generic_course_audit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(generic_course_audit PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME two_reporters_real_course COMMAND generic_course_audit
    --root ${CMAKE_CURRENT_LIST_DIR}/../../..)
add_test(NAME invalid_root_rejected COMMAND generic_course_audit
    --root ${CMAKE_CURRENT_BINARY_DIR}/not-a-course)
set_tests_properties(invalid_root_rejected PROPERTIES WILL_FAIL TRUE)
add_test(NAME unmet_reporter_requirement
    COMMAND ${CMAKE_COMMAND}
        -DCXX=${CMAKE_CXX_COMPILER}
        -DINCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -DSOURCE=${CMAKE_CURRENT_SOURCE_DIR}/negative/bad_reporter.cpp
        -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}/negative
        -P ${CMAKE_CURRENT_SOURCE_DIR}/expect_compile_failure.cmake)
