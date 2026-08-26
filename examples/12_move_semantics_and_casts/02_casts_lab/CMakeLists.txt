cmake_minimum_required(VERSION 3.16)
project(casts_lab LANGUAGES CXX)

add_executable(casts_lab casts_lab.cpp)
target_compile_features(casts_lab PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        casts_lab
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS static dynamic const reinterpret upcast)
    add_test(
        NAME cast_${case_name}
        COMMAND casts_lab --case ${case_name}
    )
endforeach()

add_test(NAME cast_all COMMAND casts_lab)
add_test(NAME cast_invalid_case COMMAND casts_lab --case missing)
set_tests_properties(cast_invalid_case PROPERTIES WILL_FAIL TRUE)
