cmake_minimum_required(VERSION 3.16)
project(constexpr_and_concepts_lab LANGUAGES CXX)

add_executable(
    constexpr_and_concepts_lab
    constexpr_and_concepts_lab.cpp
)

target_compile_features(
    constexpr_and_concepts_lab
    PRIVATE
    cxx_std_20
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        constexpr_and_concepts_lab
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS constexpr consteval constinit concepts overload)
    add_test(
        NAME modern_${case_name}
        COMMAND constexpr_and_concepts_lab --case ${case_name}
    )
endforeach()

add_test(NAME modern_all COMMAND constexpr_and_concepts_lab)
add_test(
    NAME modern_invalid_case
    COMMAND constexpr_and_concepts_lab --case missing
)
set_tests_properties(modern_invalid_case PROPERTIES WILL_FAIL TRUE)
