cmake_minimum_required(VERSION 3.10)
project(SR)

set(CMAKE_CXX_STANDARD 20)  # Update to C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

add_executable(SR src/main.cpp)