#pragma once

#include <vector>
#include <glm/glm.hpp>

bool loadOBJ(
    const char *path,
    std::vector<glm::vec3> &out_vertices,
    std::vector<glm::vec3> &out_normals,
    std::vector<glm::ivec3> &faces
);