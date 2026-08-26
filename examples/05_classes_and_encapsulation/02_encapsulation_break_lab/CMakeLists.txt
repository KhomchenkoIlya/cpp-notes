cmake_minimum_required(VERSION 3.16)
project(encapsulation_break_lab LANGUAGES CXX)

add_executable(encapsulation_break_lab encapsulation_break_lab.cpp)
target_compile_features(encapsulation_break_lab PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(encapsulation_break_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME encapsulation_positive COMMAND encapsulation_break_lab)

foreach(case_name IN ITEMS private_access const_mutation)
    add_test(NAME negative_${case_name}
        COMMAND ${CMAKE_COMMAND}
            -DCXX=${CMAKE_CXX_COMPILER}
            -DSOURCE=${CMAKE_CURRENT_LIST_DIR}/negative/${case_name}.cpp
            -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${case_name}.log
            -P ${CMAKE_CURRENT_LIST_DIR}/expect_compile_failure.cmake)
endforeach()
