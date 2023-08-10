#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cassert>
#include "color.h"
#include "print.h"
#include "framebuffer.h"
#include "uniforms.h"
#include "shaders.h"
#include "fragment.h"
#include "triangle.h"
#include "camera.h"


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

std::vector<std::vector<Vertex>> primitiveAssembly(
    Primitive polygon,
    const std::vector<Vertex>& transformedVertices
) {
    std::vector<std::vector<Vertex>> assembledVertices;

    switch (polygon) {
        case Primitive::TRIANGLES: {
            assert(transformedVertices.size() % 3 == 0 && "The number of vertices must be a multiple of 3 for triangles.");
            
            for (size_t i = 0; i < transformedVertices.size(); i += 3) {
                std::vector<Vertex> triangle = {
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

std::vector<Fragment> rasterize(Primitive primitive, const std::vector<std::vector<Vertex>>& assembledVertices) {
    std::vector<Fragment> fragments;

    switch (primitive) {
        case Primitive::TRIANGLES: {
            for (const std::vector<Vertex>& triangleVertices : assembledVertices) {
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

void render(Primitive polygon, const std::vector<glm::vec3>& VBO, const Uniforms& uniforms) {
    // 1. Vertex Shader
    std::vector<Vertex> transformedVertices;
    for (int i = 0; i < VBO.size(); i+=2) {
        glm::vec3 v = VBO[i];
        glm::vec3 c = VBO[i+1];

        Color color = Color(c.x, c.y, c.z);
        Vertex vertex = { v, color };
        transformedVertices.push_back(vertexShader(vertex, uniforms));
    }

    // 2. Primitive Assembly
    std::vector<std::vector<Vertex>> assembledVertices = primitiveAssembly(polygon, transformedVertices);

    // 3. Rasterization
    std::vector<Fragment> fragments = rasterize(polygon, assembledVertices);

    // 4. Fragment Shader
    for (const Fragment& fragment : fragments) {
        // Apply the fragment shader to compute the final color
        Color color = fragmentShader(fragment);
        point(fragment);
    }
}

glm::mat4 createViewportMatrix(size_t screenWidth, size_t screenHeight) {
    glm::mat4 viewport = glm::mat4(1.0f);

    // Scale
    viewport = glm::scale(viewport, glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, 0.5f));

    // Translate
    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
}

int main(int argc, char* argv[]) {
    if (!init()) {
        return 1;
    }

    std::vector<glm::vec3> vertexBufferObject = {
        {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f},     // Vertex 1 (top), position, color
        {-0.87f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f},   // Vertex 2 (bottom left)
        {0.87f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f},     // Vertex 3 (bottom right)


        {1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f},      // Vertex 1 (top)
        {-0.87f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f},   // Vertex 2 (bottom left)
        {0.87f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}     // Vertex 3 (bottom right)
    };

    Uniforms uniforms;

    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    glm::vec3 translationVector(0.0f, 0.0f, 0.0f);
    float a = 45.0f;
    glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis
    glm::vec3 scaleFactor(1.0f, 1.0f, 1.0f);

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), translationVector);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaleFactor);

    // Initialize a Camera object
    Camera camera;
    camera.cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
    camera.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    // Projection matrix
    float fovInDegrees = 45.0f;
    float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT; // Assuming a screen resolution of 800x600
    float nearClip = 0.1f;
    float farClip = 100.0f;
    uniforms.projection = glm::perspective(glm::radians(fovInDegrees), aspectRatio, nearClip, farClip);

    uniforms.viewport = createViewportMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        a += 0.1;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(a), rotationAxis);

        // Calculate the model matrix
        uniforms.model = translation * rotation * scale;

        // Create the view matrix using the Camera object
        uniforms.view = glm::lookAt(
            camera.cameraPosition, // The position of the camera
            camera.targetPosition, // The point the camera is looking at
            camera.upVector        // The up vector defining the camera's orientation
        );

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        clearFramebuffer();

        setColor(Color(255, 255, 0));
        render(Primitive::TRIANGLES, vertexBufferObject, uniforms);

        renderBuffer(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}