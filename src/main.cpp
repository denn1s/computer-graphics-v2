#include <SDL2/SDL.h>
#include <iostream>
#include "color.h"
#include "framebuffer.h"

Framebuffer framebuffer;
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

void point(int x, int y, const Color& color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        framebuffer[y][x] = color;
    }
}

// Initialization function, point function, and other helper functions go here

void setColor(const Color& color) {
    currentColor = color;
}

void line(glm::ivec2 p1, glm::ivec2 p2) {
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
        point(glm::ivec2(x, y));

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
void triangle(glm::ivec2 A, glm::ivec2 B, glm::ivec2 C) {
    line(A, B);
    line(B, C);
    line(C, A);
}

void drawTriangles(const std::vector<glm::ivec2>& vertices) {
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

    std::vector<glm::ivec2> vertices = {
        {300, 200},
        {400, 400},
        {500, 200}
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

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}