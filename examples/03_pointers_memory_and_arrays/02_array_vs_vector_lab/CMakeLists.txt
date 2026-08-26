cmake_minimum_required(VERSION 3.16)

project(array_vs_vector_lab LANGUAGES CXX)

function(configure_target target)
    target_compile_features(${target} PRIVATE cxx_std_17)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_executable(array_vs_vector_lab safe_lab.cpp)
configure_target(array_vs_vector_lab)

option(ENABLE_SANITIZER_CASES "Build intentionally invalid memory cases" OFF)
if(ENABLE_SANITIZER_CASES)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "These sanitizer cases require GCC or Clang")
    endif()
    foreach(case_name raw_oob dangling vector_invalidated)
        add_executable(${case_name} bugs/${case_name}.cpp)
        configure_target(${case_name})
        target_compile_options(${case_name} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${case_name} PRIVATE -fsanitize=address,undefined)
    endforeach()
endif()

enable_testing()
add_test(NAME safe_storage_lab COMMAND array_vs_vector_lab)
