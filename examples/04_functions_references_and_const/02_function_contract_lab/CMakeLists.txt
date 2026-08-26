cmake_minimum_required(VERSION 3.16)
project(function_contract_lab LANGUAGES CXX)

option(ENABLE_SANITIZER_CASE "Build the intentionally unsafe dangling-return case" OFF)

add_executable(function_contract_lab function_contract_lab.cpp)
target_compile_features(function_contract_lab PRIVATE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(function_contract_lab PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME contracts_positive COMMAND function_contract_lab)

foreach(case_name IN ITEMS const_mutation return_type_only ambiguous_overload)
    add_test(NAME negative_${case_name}
        COMMAND ${CMAKE_COMMAND}
            -DCXX=${CMAKE_CXX_COMPILER}
            -DSOURCE=${CMAKE_CURRENT_LIST_DIR}/negative/${case_name}.cpp
            -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${case_name}.log
            -P ${CMAKE_CURRENT_LIST_DIR}/expect_compile_failure.cmake)
endforeach()

if(ENABLE_SANITIZER_CASE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_executable(dangling_return unsafe/dangling_return.cpp)
        target_compile_features(dangling_return PRIVATE cxx_std_17)
        target_compile_options(dangling_return PRIVATE -Wall -Wextra -Wpedantic
            -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(dangling_return PRIVATE -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "ENABLE_SANITIZER_CASE requires GCC or Clang")
    endif()
endif()
