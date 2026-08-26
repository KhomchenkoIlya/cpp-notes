cmake_minimum_required(VERSION 3.16)
project(cpp20_feature_tour LANGUAGES CXX)

add_executable(cpp20_feature_tour cpp20_feature_tour.cpp)
target_compile_features(cpp20_feature_tour PRIVATE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        cpp20_feature_tour
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

enable_testing()

foreach(case_name IN ITEMS ranges span designated comparison erase_if starts_with)
    add_test(
        NAME cpp20_${case_name}
        COMMAND cpp20_feature_tour --case ${case_name}
    )
endforeach()

add_test(NAME cpp20_all COMMAND cpp20_feature_tour)
add_test(
    NAME cpp20_invalid_case
    COMMAND cpp20_feature_tour --case missing
)
set_tests_properties(cpp20_invalid_case PROPERTIES WILL_FAIL TRUE)
