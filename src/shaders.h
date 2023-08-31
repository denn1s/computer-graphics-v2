#pragma once
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include "uniforms.h"
#include "fragment.h"
#include "noise.h"
#include "print.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    glm::vec3 transformedWorldPosition = glm::vec3(uniforms.model * glm::vec4(vertex.position, 1.0f));

    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
        vertex.tex,
        transformedWorldPosition,
        vertex.position
    };
}

Fragment fragmentShaderStripes(Fragment& fragment) {
        // Define the base color for Mars

    // Create some noise based on the fragment position
    /* float noise = glm::fract(sin(glm::dot(glm::vec2(fragment.x, fragment.y), glm::vec2(12.9898f, 78.233f))) * 43758.5453f); */

    /* float noise = glm::fract(sin(glm::dot(fragment.worldPos, glm::vec3(12.9898f, 78.233f, 56.789f))) * 43758.5453f); */
    // Slightly adjust the color based on the noise
    /* Color color = marsColor + Color(static_cast<int>(noise * 25), static_cast<int>(noise * 10), static_cast<int>(noise * 5)); */
    /* glm::vec3 wc = fragment.worldPos * 1.0f + 0.5f; */
    /* Color color = Color(wc.r, wc.g, wc.b); */

    // Define a color for Jupiter. You may want to adjust this to better match Jupiter's appearance.
    Color baseColor = Color(125, 67, 37);  // This color is a kind of dark orange

    // Create stripes based on the fragment's Y position in world space. The frequency of the stripes 
    // can be controlled by scaling the Y coordinate. 
    // The number 10.0f determines the frequency of the stripes, adjust it to get the desired effect.
    float stripePattern = glm::abs(glm::cos(fragment.originalPos.y * 20.0f));

    // Interpolate between the base color and a darker version of the base color based on the stripe pattern.
    // This will create dark stripes on the sphere.
    Color stripeColor = baseColor * (0.5f + 0.5f * stripePattern);

    // Apply lighting intensity
    stripeColor = stripeColor * fragment.intensity;

    // Set the fragment color
    Color color = stripeColor;



    // Define the direction to the center of the circle in world space
    // Apply lighting intensity
    color = color * fragment.intensity;

    // Set the fragment color
    fragment.color = color;

    return fragment;
}

Fragment fragmentShaderNorthPole(Fragment& fragment) {
    // Define a color for the ocean and the continent
    glm::vec3 oceanColor = glm::vec3(0, 0, 128);  // Dark blue
    glm::vec3 continentColor = glm::vec3(34, 139, 34);  // Forest green

    // Create a simple "continent" in the northern hemisphere by making a region where the Y coordinate is 
    // above a certain value look different
    float continentPattern = glm::smoothstep(0.4f, 1.0f, fragment.originalPos.y);

    // Interpolate between the ocean color and the continent color based on the continent pattern
    glm::vec3 c = glm::mix(oceanColor, continentColor, continentPattern);
    Color color = Color(c.x, c.y, c.z);
    // Apply lighting intensity
    color = color * fragment.intensity;

    // Set the fragment color
    fragment.color = color;

    return fragment;
}

Fragment fragmentShaderGreeN(Fragment& fragment) {
    // Define the color for the ocean and the continent using integer values
    Color oceanColor = Color(0, 0, 128);  // Dark blue
    Color continentColor = Color(34, 139, 34);  // Forest green

    // Create a simple "continent" using a spot on the side of the planet.
    // The spot is defined as a region where the x coordinate is within a certain range.
    float continentPattern = glm::smoothstep(-0.2f, 0.2f, fragment.originalPos.x);

    // Interpolate between the ocean color and the continent color based on the continent pattern
    Color c = oceanColor * (1.0f - continentPattern) + continentColor * continentPattern;

    // Apply lighting intensity
    c = c * fragment.intensity;

    // Set the fragment color
    fragment.color = c;

    return fragment;
}

Fragment fragmentShader(Fragment& fragment) {
    // Define the colors for the ocean, the ground, and the spots with direct values
    glm::vec3 spotColorGreen = glm::vec3(0.133f, 0.545f, 0.133f);  // Forest green
    glm::vec3 spotColorBlue = glm::vec3(0.0f, 0.0f, 1.0f);  // Blue

    // Sample the Perlin noise map at the fragment's position
    glm::vec2 uv = glm::vec2(fragment.originalPos.x, fragment.originalPos.z);
    uv = glm::clamp(uv, 0.0f, 1.0f);  // make sure the uv coordinates are in [0, 1] range

    // Set up the noise generator
    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    float ox = 1200.0f;
    float oy = 3000.0f;
    float z = 1000.0f;
    // Generate the noise value
    float noiseValue = noiseGenerator.GetNoise((uv.x + ox) * z, (uv.y + oy) * z);

    // Decide the spot color based on the noise value
    glm::vec3 c = (noiseValue < 0.5f) ? spotColorBlue : spotColorGreen;

    // Interpolate between the ocean color and the ground color based on the water/ground transition
    // Then, interpolate between this color and the spot color

    // Convert glm::vec3 color to your Color class
    fragment.color = Color(c.r, c.g, c.b);

    return fragment;
}
