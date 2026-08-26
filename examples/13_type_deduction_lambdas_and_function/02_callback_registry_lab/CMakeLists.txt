cmake_minimum_required(VERSION 3.16)
project(callback_registry_lab LANGUAGES CXX)

add_executable(callback_registry_lab callback_registry_lab.cpp)
target_compile_features(callback_registry_lab PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        callback_registry_lab
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS heterogeneous mutable weak empty template)
    add_test(
        NAME callback_${case_name}
        COMMAND callback_registry_lab --case ${case_name}
    )
endforeach()

add_test(NAME callback_all COMMAND callback_registry_lab)
add_test(
    NAME callback_invalid_case
    COMMAND callback_registry_lab --case missing
)
set_tests_properties(callback_invalid_case PROPERTIES WILL_FAIL TRUE)
