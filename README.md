# Software Renderer (SR)

SR is a simple C++ project that demonstrates the use of C++20 features and the GLM library. The main goal of this project is to serve as a starting point for a software renderer. At this stage, it prints a "Hello, World!" message along with a `glm::vec3` and a `glm::mat4`.

## Contents

- `src/main.cpp`: The main file that includes the `print.h` header and uses the `print` function to display the "Hello, World!" message, a `glm::vec3`, and a `glm::mat4`.
- `src/print.h`: A header file that contains overloaded `print` functions for different types, including `glm::vec3` and `glm::mat4`. It also provides a variadic template `print` function.

## How to Compile and Run

1. Ensure you have the required dependencies installed: `glm` library.
2. Create a build directory and navigate to it:

   ```bash
   mkdir build
   cd build
   ```

3. Generate the build system using CMake and compile the program:

   ```bash
   cmake ..
   make
   ```

4. Run the compiled executable:

   ```bash
   ./SR
   ```

The output should display the "Hello, World!" message along with a `glm::vec3` and a `glm::mat4`.

