cmake_minimum_required(VERSION 3.16)
project(course_inventory_classes LANGUAGES CXX)

add_executable(course_inventory_classes course_inventory_classes.cpp)
target_compile_features(course_inventory_classes PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(course_inventory_classes PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME inventory_all
    COMMAND course_inventory_classes --root ${CMAKE_CURRENT_LIST_DIR}/../../..)
add_test(NAME inventory_one
    COMMAND course_inventory_classes --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --block 3)
add_test(NAME reject_out_of_range
    COMMAND course_inventory_classes --root ${CMAKE_CURRENT_LIST_DIR}/../../.. --block 16)
set_tests_properties(reject_out_of_range PROPERTIES WILL_FAIL TRUE)
