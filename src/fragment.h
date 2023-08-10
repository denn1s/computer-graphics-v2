#pragma once
#include "color.h"

struct Vertex {
    glm::vec3 position;
    Color color;
};

struct Fragment {
    glm::ivec2 position; // X and Y coordinates of the pixel (in screen space)
    glm::ivec3 color; // r, g, b values for color
    // Other interpolated attributes (e.g., color, texture coordinates, normals) can be added here
};