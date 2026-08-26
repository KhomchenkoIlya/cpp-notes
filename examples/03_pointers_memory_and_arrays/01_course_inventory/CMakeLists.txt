cmake_minimum_required(VERSION 3.16)

project(course_inventory LANGUAGES CXX)

add_executable(course_inventory course_inventory.cpp)
target_compile_features(course_inventory PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(course_inventory PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME inventory_all COMMAND course_inventory --root ${CMAKE_CURRENT_LIST_DIR}/../../..)
add_test(NAME inventory_one COMMAND course_inventory --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --block 3)
