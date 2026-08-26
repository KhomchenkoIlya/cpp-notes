cmake_minimum_required(VERSION 3.16)

# Проект использует C++.
project(basic_abstractions LANGUAGES CXX)

# Одна исполняемая цель собирается из main.cpp.
add_executable(basic_abstractions main.cpp)

# Требуем C++17 именно для этой цели.
target_compile_features(basic_abstractions PRIVATE cxx_std_17)

# На GCC/Clang включаем базовый набор полезных warnings.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(basic_abstractions PRIVATE -Wall -Wextra -Wpedantic)
endif()
