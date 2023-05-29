cmake_minimum_required(VERSION 3.26)
project(CG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 and glm
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

# Include the directories with headers
include_directories(${SDL2_INCLUDE_DIRS})

# Add the executable
add_executable(CG src/main.cpp)

# Link against SDL2 and glm
target_link_libraries(CG ${SDL2_LIBRARIES} glm)
