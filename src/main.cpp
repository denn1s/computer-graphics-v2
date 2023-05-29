#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* args[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Rotation Demo",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Point triangle[4];
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    glm::vec3 points[3] = { 
        {centerX - 50, centerY - 50, 0.0f}, 
        {centerX + 50, centerY - 50, 0.0f}, 
        {centerX, centerY + 50, 0.0f} 
    };

    bool isRunning = true;
    SDL_Event event;
    float rotateAngle = 0.0f;

    unsigned int lastTime = SDL_GetTicks();
    unsigned int currentTime;
    float dT;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Calculate the deltaTime
        currentTime = SDL_GetTicks();
        dT = (currentTime - lastTime) / 1000.0f;  // Time since last frame in seconds
        lastTime = currentTime;

        // Apply rotation
        rotateAngle += glm::radians(120.0f) * dT;  // Rotate 120 degrees per second
        glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(-centerX, -centerY, 0.0f));
        glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translationToCenter = glm::translate(glm::mat4(1.0f), glm::vec3(centerX, centerY, 0.0f));

        // Rotate and translate each point of the triangle
        for (int i = 0; i < 3; i++) {
            glm::vec4 transformedPoint = translationToCenter * rotationMat * translationToOrigin * glm::vec4(points[i], 1.0f);
            triangle[i].x = transformedPoint.x;
            triangle[i].y = transformedPoint.y;
        }

        triangle[3] = triangle[0];  // Close the triangle

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLines(renderer, triangle, 4);  // Render the closed triangle
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
