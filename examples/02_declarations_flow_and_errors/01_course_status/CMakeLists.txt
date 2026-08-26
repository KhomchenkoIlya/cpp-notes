cmake_minimum_required(VERSION 3.16)

project(course_status LANGUAGES CXX)

add_executable(course_status course_status.cpp)
target_compile_features(course_status PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(course_status PRIVATE -Wall -Wextra -Wpedantic)
endif()
