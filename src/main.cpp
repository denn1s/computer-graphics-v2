#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <cassert>
#include "color.h"
#include "print.h"
#include "framebuffer.h"
#include "uniforms.h"
#include "shaders.h"
#include "fragment.h"
#include "triangle.h"


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Color currentColor;

enum class Primitive {
    TRIANGLES,
    // Add more primitive types here (e.g., SQUARES, LINES, etc.)
};

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error: Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error: Failed to create SDL window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error: Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void setColor(const Color& color) {
    currentColor = color;
}

std::vector<std::vector<glm::vec3>> primitiveAssembly(
    Primitive polygon,
    const std::vector<glm::vec3>& transformedVertices
) {
    std::vector<std::vector<glm::vec3>> assembledVertices;

    switch (polygon) {
        case Primitive::TRIANGLES: {
            assert(transformedVertices.size() % 3 == 0 && "The number of vertices must be a multiple of 3 for triangles.");

            for (size_t i = 0; i < transformedVertices.size(); i += 3) {
                std::vector<glm::vec3> triangle = {
                    transformedVertices[i],
                    transformedVertices[i + 1],
                    transformedVertices[i + 2]
                };
                assembledVertices.push_back(triangle);
            }
            break;
        }
        // Add more cases for other primitive types here
        default:
            std::cerr << "Error: Unsupported primitive type." << std::endl;
            break;
    }

    return assembledVertices;
}

std::vector<Fragment> rasterize(Primitive primitive, const std::vector<std::vector<glm::vec3>>& assembledVertices) {
    std::vector<Fragment> fragments;

    switch (primitive) {
        case Primitive::TRIANGLES: {
            for (const std::vector<glm::vec3>& triangleVertices : assembledVertices) {
                assert(triangleVertices.size() == 3 && "Triangle vertices must contain exactly 3 vertices.");
                std::vector<Fragment> triangleFragments = triangle(triangleVertices[0], triangleVertices[1], triangleVertices[2]);
                fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
            }
            break;
        }
        // Add more cases for other primitive types here (e.g., LINES, POINTS, etc.)
        default:
            std::cerr << "Error: Unsupported primitive type for rasterization." << std::endl;
            break;
    }

    return fragments;
}

void render(Primitive polygon, const std::vector<glm::vec3>& vertices, const Uniforms& uniforms) {
    // 1. Vertex Shader
    std::vector<glm::vec3> transformedVertices;
    for (const glm::vec3& vertex : vertices) {
        transformedVertices.push_back(vertexShader(vertex, uniforms));
    }

    // 2. Primitive Assembly
    std::vector<std::vector<glm::vec3>> assembledVertices = primitiveAssembly(polygon, transformedVertices);

    // 3. Rasterization
    std::vector<Fragment> fragments = rasterize(polygon, assembledVertices);

    // 4. Fragment Shader
    for (const Fragment& fragment : fragments) {
        // Apply the fragment shader to compute the final color
        Color color = fragmentShader(fragment);
        framebuffer[fragment.position.y][fragment.position.x] = color;
    }
}

int main(int argc, char* argv[]) {
    if (!init()) {
        return 1;
    }

    std::vector<glm::vec3> vertices = {
        {300.0f, 200.0f, 0.0f},
        {400.0f, 400.0f, 0.0f},
        {500.0f, 200.0f, 0.0f}
    };

    Uniforms uniforms;

    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    uniforms.model = model;
    uniforms.model = view;
    uniforms.model = projection;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        setColor(Color(255, 255, 0));
        render(Primitive::TRIANGLES, vertices, uniforms);

        renderBuffer(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}