#pragma once
#include "color.h"

struct Vertex {
    glm::vec3 position;
    Color color;
};

struct Fragment {
    glm::ivec2 position; // X and Y coordinates of the pixel (in screen space)
    Color color; // r, g, b values for color
    double z;  // zbuffer
    float intensity;  // light intensity
};