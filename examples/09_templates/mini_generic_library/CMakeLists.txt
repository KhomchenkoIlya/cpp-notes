cmake_minimum_required(VERSION 3.16)
project(mini_generic_library LANGUAGES CXX)

add_executable(mini_generic_library mini_generic_library.cpp)
target_compile_features(mini_generic_library PRIVATE cxx_std_17)
target_include_directories(mini_generic_library PRIVATE include)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mini_generic_library PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME generic_positive COMMAND mini_generic_library)
add_test(NAME unmet_less_requirement
    COMMAND ${CMAKE_COMMAND}
        -DCXX=${CMAKE_CXX_COMPILER}
        -DINCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/include
        -DSOURCE=${CMAKE_CURRENT_SOURCE_DIR}/negative/no_less.cpp
        -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}/negative
        -P ${CMAKE_CURRENT_SOURCE_DIR}/expect_compile_failure.cmake)
