#pragma once
#include <glm/glm.hpp>
#include "uniforms.h"
#include "fragment.h"

#include "print.h"


Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;


    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        vertex.color
    };
}

Fragment fragmentShader(Fragment& fragment) {
    // Example: Assign a constant color to each fragment

    fragment.color = fragment.color * fragment.intensity; // Red color with full opacity
    // You can modify this function to implement more complex shading
    // based on the fragment's attributes (e.g., depth, interpolated normals, texture coordinates, etc.)

    return fragment;
}