#pragma once

#include <glm/glm.hpp>
#include "intersect.h"
#include "material.h"

class Object {
public:
    Object() {}
    virtual ~Object() = default;

    // A pure virtual function providing interface framework.
    virtual Intersect rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const = 0;

    Material* material = nullptr;
};
