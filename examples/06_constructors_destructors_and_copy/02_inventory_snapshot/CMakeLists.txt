cmake_minimum_required(VERSION 3.16)
project(inventory_snapshot LANGUAGES CXX)

add_executable(inventory_snapshot inventory_snapshot.cpp)
target_compile_features(inventory_snapshot PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(inventory_snapshot PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME snapshot_all
    COMMAND inventory_snapshot --root ${CMAKE_CURRENT_LIST_DIR}/../../..)
add_test(NAME snapshot_rejects_wrong_root
    COMMAND inventory_snapshot --root ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(snapshot_rejects_wrong_root PROPERTIES WILL_FAIL TRUE)
