#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include "color.h"

// overload for glm::vec3
void print(const glm::vec3& v) {
    std::cout << "glm::vec3(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

// overload for glm::vec4
void print(const glm::vec4& v) {
    std::cout << "glm::vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
}

// overload for glm::ivec2
void print(const glm::ivec2& v) {
    std::cout << "glm::vec2(" << v.x << ", " << v.y << ")" << std::endl;
}

// overload for Color
void print(const Color& c) {
    std::cout << "Color("
        << static_cast<int>(c.r) 
        << ", "
        << static_cast<int>(c.g) 
        << ", "
        << static_cast<int>(c.b) 
        << ")"
    << std::endl;
}

// overload for glm::mat4
void print(const glm::mat4& m) {
    std::cout << "glm::mat4(\n";
    for(int i = 0; i < 4; ++i) {
        std::cout << "  ";
        for(int j = 0; j < 4; ++j) {
            std::cout << m[i][j];
            if (j != 3) {
                std::cout << ", ";
            }
        }
        std::cout << (i == 3 ? "\n" : ",\n");
    }
    std::cout << ")" << std::endl;
}

// base case function to end the recursion, using abbreviated template syntax
void print(auto last) {
    std::cout << last << '\n';
}

// recursive variadic template function
template <typename T, typename... Args>
void print(T first, Args... args) {
    print(first);  // call the appropriate print function
    if(sizeof...(args) > 0)
        std::cout << ' ';  // print a space only if there are more arguments
    print(args...);  // call print with remaining arguments
}
