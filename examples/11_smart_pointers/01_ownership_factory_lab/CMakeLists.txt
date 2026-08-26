cmake_minimum_required(VERSION 3.16)
project(ownership_factory_lab LANGUAGES CXX)

add_executable(ownership_factory_lab ownership_factory_lab.cpp)
target_compile_features(ownership_factory_lab PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ownership_factory_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()

foreach(case_name IN ITEMS factory transfer borrowing reset)
    add_test(
        NAME ownership_${case_name}
        COMMAND ownership_factory_lab --case ${case_name}
    )
endforeach()

add_test(NAME ownership_all COMMAND ownership_factory_lab)
add_test(NAME ownership_invalid_case COMMAND ownership_factory_lab --case missing)
set_tests_properties(ownership_invalid_case PROPERTIES WILL_FAIL TRUE)
