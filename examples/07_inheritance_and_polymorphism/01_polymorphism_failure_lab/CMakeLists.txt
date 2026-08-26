cmake_minimum_required(VERSION 3.16)
project(polymorphism_failure_lab LANGUAGES CXX)

option(ENABLE_UNSAFE_CASE "Build isolated non-virtual-destructor UB case" OFF)

add_executable(polymorphism_failure_lab polymorphism_failure_lab.cpp)
target_compile_features(polymorphism_failure_lab PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(polymorphism_failure_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME polymorphism_safe COMMAND polymorphism_failure_lab)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    foreach(case IN ITEMS wrong_override abstract_instantiation private_base_access)
        if(case STREQUAL "wrong_override")
            set(pattern "override|does not override")
        elseif(case STREQUAL "abstract_instantiation")
            set(pattern "abstract")
        else()
            set(pattern "private")
        endif()
        add_test(NAME negative_${case}
            COMMAND ${CMAKE_COMMAND}
                -DCXX=${CMAKE_CXX_COMPILER}
                -DSOURCE=${CMAKE_CURRENT_LIST_DIR}/negative/${case}.cpp
                -DOBJECT=${CMAKE_CURRENT_BINARY_DIR}/${case}.o
                -DLOG=${CMAKE_CURRENT_BINARY_DIR}/${case}.log
                -DPATTERN=${pattern}
                -P ${CMAKE_CURRENT_LIST_DIR}/expect_compile_failure.cmake)
    endforeach()
endif()

if(ENABLE_UNSAFE_CASE)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "ENABLE_UNSAFE_CASE currently supports GCC and Clang only")
    endif()
    target_compile_options(polymorphism_failure_lab PRIVATE
        -g -fno-omit-frame-pointer -fsanitize=address,undefined)
    target_link_options(polymorphism_failure_lab PRIVATE
        -fno-omit-frame-pointer -fsanitize=address,undefined)
    add_executable(unsafe_nonvirtual_delete unsafe_nonvirtual_delete.cpp)
    target_compile_features(unsafe_nonvirtual_delete PRIVATE cxx_std_17)
    target_compile_options(unsafe_nonvirtual_delete PRIVATE
        -Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer -fsanitize=address,undefined)
    target_link_options(unsafe_nonvirtual_delete PRIVATE
        -fno-omit-frame-pointer -fsanitize=address,undefined)
    add_test(NAME unsafe_nonvirtual_delete_expected_failure
        COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:unsafe_nonvirtual_delete>
            -DLOG=${CMAKE_CURRENT_BINARY_DIR}/unsafe_nonvirtual_delete.log
            -P ${CMAKE_CURRENT_LIST_DIR}/expect_unsafe_failure.cmake)
endif()
