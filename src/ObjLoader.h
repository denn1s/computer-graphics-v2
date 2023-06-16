#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Face
{
    std::vector<std::array<int, 3>> vertexIndices;  // [vertex, normal] indices
};


bool loadOBJ(
    const char *path,
    std::vector<glm::vec3> &out_vertices,
    std::vector<glm::vec3> &out_normals,
    std::vector<Face>& out_faces,
    std::vector<std::string>& out_materials
);