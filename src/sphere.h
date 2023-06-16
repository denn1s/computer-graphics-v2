#pragma once

#include <glm/glm.hpp>
#include "object.h"

class Sphere : public Object {
  public:
      Sphere(glm::vec3 center, float radius);

      bool rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const override;

  private:
      glm::vec3 center;
      float radius;
};