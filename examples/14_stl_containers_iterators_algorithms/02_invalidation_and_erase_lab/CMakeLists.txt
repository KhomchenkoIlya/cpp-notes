cmake_minimum_required(VERSION 3.16)
project(invalidation_and_erase_lab LANGUAGES CXX)

add_executable(invalidation_and_erase_lab invalidation_and_erase_lab.cpp)
target_compile_features(invalidation_and_erase_lab PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        invalidation_and_erase_lab
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS reserve reallocation erase_remove erase_return list rehash)
    add_test(
        NAME invalidation_${case_name}
        COMMAND invalidation_and_erase_lab --case ${case_name}
    )
endforeach()

add_test(NAME invalidation_all COMMAND invalidation_and_erase_lab)
add_test(
    NAME invalidation_invalid_case
    COMMAND invalidation_and_erase_lab --case missing
)
set_tests_properties(invalidation_invalid_case PROPERTIES WILL_FAIL TRUE)
