#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <sstream>
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


void render(const std::vector<glm::vec3>& VBO, const Uniforms& uniforms) {
    // 1. Vertex Shader
    std::vector<Vertex> transformedVertices(VBO.size() / 2);

    tbb::parallel_for(tbb::blocked_range<size_t>(0, VBO.size() / 2),
        [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); ++i) {
                glm::vec3 v = VBO[2 * i];
                glm::vec3 c = VBO[2 * i + 1];

                Color color = Color(c.x, c.y, c.z);
                Vertex vertex = { v, color };

                transformedVertices[i] = vertexShader(vertex, uniforms);
            }
        }
    );

    // 2. Primitive Assembly
    std::vector<std::vector<Vertex>> assembledVertices(transformedVertices.size() / 3);

    tbb::parallel_for(tbb::blocked_range<size_t>(0, transformedVertices.size() / 3),
        [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); ++i) {
                Vertex edge1 = transformedVertices[3 * i];
                Vertex edge2 = transformedVertices[3 * i + 1];
                Vertex edge3 = transformedVertices[3 * i + 2];

                assembledVertices[i] =  { edge1, edge2, edge3 };
            }
        }
    );

    // 3. Rasterization
    tbb::concurrent_vector<Fragment> concurrentFragments;

    tbb::parallel_for(tbb::blocked_range<size_t>(0, assembledVertices.size()),
        [&](const tbb::blocked_range<size_t>& r) {
            for (size_t i = r.begin(); i != r.end(); ++i) {
                std::vector<Fragment> rasterizedTriangle = triangle(
                    assembledVertices[i][0],
                    assembledVertices[i][1],
                    assembledVertices[i][2]
                );

                for(const auto& fragment : rasterizedTriangle) {
                    concurrentFragments.push_back(fragment);
                }
            }
        }
    );

    // 4. Fragment Shader
    tbb::parallel_for(tbb::blocked_range<size_t>(0, concurrentFragments.size()),
        [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i != range.end(); ++i) {
                const Fragment& fragment = fragmentShader(concurrentFragments[i]);
                // Apply the fragment shader to compute the final color

                point(fragment);  // Be aware of potential race conditions here
            }
        }
    );
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



    Uint32 frameStart, frameTime;
    std::string title = "FPS: ";

    bool running = true;
    while (running) {
        frameStart = SDL_GetTicks();

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
        render(vertexBufferObject, uniforms);

        renderBuffer(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        // Calculate frames per second and update window title
        if (frameTime > 0) {
            std::ostringstream titleStream;
            titleStream << "FPS: " << 1000.0 / frameTime;  // Milliseconds to seconds
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}