cmake_minimum_required(VERSION 3.26)
project(CG)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

# Gather all the cpp files
file(GLOB SOURCE_FILES
        "src/*.cpp")


# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# specify the required features 
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Copy models
# file(COPY ${CMAKE_SOURCE_DIR}/src/models DESTINATION ${CMAKE_BINARY_DIR})


# Link libraries to the executable
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
