#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "line.h"

glm::vec3 barycentricCoordinates(const glm::vec3& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {
    // The formula used is directly derived from the area of triangles. It uses the areas of sub-triangles PBC, PCA, and PAB
    // to calculate the barycentric coordinates. The equations for 'w' and 'v' are derived from the ratios of these areas to the area of ABC.

    // w is the weight of vertex A. It is calculated as the ratio of the area of triangle PBC to ABC.
    // It is obtained by subtracting the y-coordinate of B from the y-coordinate of C, 
    // multiplying it with the difference of the x-coordinate of P and the x-coordinate of C,
    // then adding the product of the difference of the x-coordinate of C and the x-coordinate of B 
    // and the difference of the y-coordinate of P and the y-coordinate of C.
    // This entire sum is divided by the determinant of the 2x2 matrix formed by the vectors AC and BC.
    float w = ((B.y - C.y)*(P.x - C.x) + (C.x - B.x)*(P.y - C.y)) /
              ((B.y - C.y)*(A.x - C.x) + (C.x - B.x)*(A.y - C.y));

    // v is the weight of vertex B. It is calculated as the ratio of the area of triangle PCA to ABC.
    // The calculation is similar to that of 'w', but using different sets of points.
    float v = ((C.y - A.y)*(P.x - C.x) + (A.x - C.x)*(P.y - C.y)) /
              ((B.y - C.y)*(A.x - C.x) + (C.x - B.x)*(A.y - C.y));

    // u is the weight of vertex C. It is calculated as 1 minus the sum of the other two weights,
    // because the barycentric coordinates must sum to 1. This property comes from the definition of barycentric coordinates.
    float u = 1.0f - w - v;

    // The function returns the barycentric coordinates as a 3D vector.
    return glm::vec3(w, v, u);
}

std::vector<Fragment> triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    std::vector<Fragment> fragments;

    // Calculate the bounding box of the triangle
    float minX = std::min(std::min(A.x, B.x), C.x);
    float minY = std::min(std::min(A.y, B.y), C.y);
    float maxX = std::max(std::max(A.x, B.x), C.x);
    float maxY = std::max(std::max(A.y, B.y), C.y);

    // Iterate over each point in the bounding box
    for (float y = minY; y <= maxY; ++y) {
        for (float x = minX; x <= maxX; ++x) {
            // Calculate barycentric coordinates for the point
            glm::vec3 P(x, y, 0.0f);
            glm::vec3 barycentric = barycentricCoordinates(P, A, B, C);

            // If the point's barycentric coordinates are all between 0 and 1, it lies within the triangle
            if (barycentric.x >= 0 && barycentric.x <= 1 && 
                barycentric.y >= 0 && barycentric.y <= 1 && 
                barycentric.z >= 0 && barycentric.z <= 1) {
                // Add the point to the fragment list
                fragments.push_back(Fragment{P});
            }
        }
    }
    return fragments;
}