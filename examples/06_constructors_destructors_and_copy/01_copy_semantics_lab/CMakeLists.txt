cmake_minimum_required(VERSION 3.16)
project(copy_semantics_lab LANGUAGES CXX)

option(ENABLE_UNSAFE_CASE "Build the intentionally broken shallow-copy case" OFF)

add_executable(copy_semantics_lab copy_semantics_lab.cpp)
target_compile_features(copy_semantics_lab PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(copy_semantics_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME copy_semantics_safe COMMAND copy_semantics_lab)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_test(NAME initializer_order_warning
        COMMAND ${CMAKE_COMMAND}
            -DCXX=${CMAKE_CXX_COMPILER}
            -DSOURCE=${CMAKE_CURRENT_LIST_DIR}/initializer_order_warning.cpp
            -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/initializer_order_warning.log
            -P ${CMAKE_CURRENT_LIST_DIR}/expect_reorder_warning.cmake)
endif()

if(ENABLE_UNSAFE_CASE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(copy_semantics_lab PRIVATE
            -g -fno-omit-frame-pointer -fsanitize=address,undefined)
        target_link_options(copy_semantics_lab PRIVATE
            -fno-omit-frame-pointer -fsanitize=address,undefined)
        add_executable(bad_shallow_copy bad_shallow_copy.cpp)
        target_compile_features(bad_shallow_copy PRIVATE cxx_std_17)
        target_compile_options(bad_shallow_copy PRIVATE
            -Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer
            -fsanitize=address,undefined)
        target_link_options(bad_shallow_copy PRIVATE
            -fno-omit-frame-pointer -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "ENABLE_UNSAFE_CASE currently supports GCC and Clang only")
    endif()
endif()
