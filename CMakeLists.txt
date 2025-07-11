cmake_minimum_required(VERSION 3.24)
project(SDL3Base)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(vendor/SDL)
add_subdirectory(src)