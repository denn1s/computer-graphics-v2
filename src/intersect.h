#pragma once

#include <glm/glm.hpp>

struct Intersect {
    glm::vec3 point;
    glm::vec3 normal;
    float distance;

    Intersect(const glm::vec3& p, const glm::vec3& n, float d) : point(p), normal(n), distance(d) {}
};

