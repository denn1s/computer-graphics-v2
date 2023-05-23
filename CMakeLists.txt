cmake_minimum_required(VERSION 3.26)
project(CG)

# Set the path to your source file
set(SOURCE_FILES src/main.cpp)

# Find the glm library
find_package(glm REQUIRED)

# Add the executable
add_executable(EXEC ${SOURCE_FILES})

# Link against the glm library
target_link_libraries(EXEC glm)
