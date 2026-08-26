cmake_minimum_required(VERSION 3.16)
project(shared_weak_graph_lab LANGUAGES CXX)

add_executable(shared_weak_graph_lab shared_weak_graph_lab.cpp)
target_compile_features(shared_weak_graph_lab PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(shared_weak_graph_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()

foreach(case_name IN ITEMS shared cycle weak lock)
    add_test(
        NAME graph_${case_name}
        COMMAND shared_weak_graph_lab --case ${case_name}
    )
endforeach()

add_test(NAME graph_all COMMAND shared_weak_graph_lab)
add_test(NAME graph_invalid_case COMMAND shared_weak_graph_lab --case missing)
set_tests_properties(graph_invalid_case PROPERTIES WILL_FAIL TRUE)
