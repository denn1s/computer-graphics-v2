#pragma once

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    float intensity;

    Light(const glm::vec3& pos, float intens) : position(pos), intensity(intens) {}
};