cmake_minimum_required(VERSION 3.5)

project(sanitizer_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# address sanitizers.
include(cmake_scripts/sanitizers.cmake)

add_executable(sanitizer_test main.cpp)
