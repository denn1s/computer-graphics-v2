#pragma once

#include <glm/glm.hpp>

class Object {
public:
    // A pure virtual function providing interface framework.
    virtual bool rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const = 0;
};
