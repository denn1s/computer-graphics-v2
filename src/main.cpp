#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "color.h"
#include "framebuffer.h"

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

void point(int x, int y) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        framebuffer[y][x] = currentColor;
    }
}

void point(float x, float y) {
    point(
        static_cast<int>(x),
        static_cast<int>(y)
    );
}

// Initialization function, point function, and other helper functions go here

void setColor(const Color& color) {
    currentColor = color;
}

void line(glm::vec3 p1, glm::vec3 p2) {
    // Calculate the absolute difference between the x and y coordinates of the two points
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);

    // Initialize the starting point (x, y) as the first point (p1)
    int x = p1.x;
    int y = p1.y;

    // Determine the step direction for x and y based on the relative positions of p1 and p2
    int x_step = (p1.x < p2.x) ? 1 : -1;
    int y_step = (p1.y < p2.y) ? 1 : -1;

    // Calculate the initial error value, which is half of the major axis (dx or dy)
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    // Iterate until the end point (p2) is reached
    while (true) {
        // Draw a pixel at the current position (x, y)
        point(x, y);

        // If the current position (x, y) matches the end point (p2), break the loop
        if (x == p2.x && y == p2.y) {
            break;
        }

        // Store the current error value in e2
        e2 = err;

        // Update the x coordinate and the error value if needed
        if (e2 > -dx) {
            err -= dy;
            x += x_step;
        }

        // Update the y coordinate and the error value if needed
        if (e2 < dy) {
            err += dx;
            y += y_step;
        }
    }
}
void triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    line(A, B);
    line(B, C);
    line(C, A);
}

void drawTriangles(const std::vector<glm::vec3>& vertices) {
    if (vertices.size() % 3 != 0) {
        std::cerr << "Error: The vertices vector must contain a multiple of 3 elements" << std::endl;
        return;
    }

    for (size_t i = 0; i < vertices.size(); i += 3) {
        triangle(vertices[i], vertices[i + 1], vertices[i + 2]);
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
        drawTriangles(vertices);

        renderBuffer(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}