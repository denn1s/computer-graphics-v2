cmake_minimum_required(VERSION 3.26)
project(CG)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Find required packages
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)  # Added line to find GLFW library

# Gather all the cpp files
file(GLOB SOURCE_FILES
        "src/*.cpp")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# specify the required features
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME} glfw)  # Added glfw to target_link_libraries
