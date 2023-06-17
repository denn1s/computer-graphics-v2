#define FOV glm::radians(90.0f)  // Field of view is 90 degrees
#define ASPECT_RATIO (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "light.h"
#include "color.h"
#include "object.h"
#include "sphere.h"
#include "camera.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Renderer* renderer = nullptr;
Light light(glm::vec3(-20, 20, 20), 1.5f);

Color castRay(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Object*>& objects) {
    Intersect intersect;
    Object* hitObject = nullptr;
    float zBuffer = std::numeric_limits<float>::infinity();

    // Check all objects for intersection
    for (auto& obj : objects) {
        Intersect tempIntersect = obj->rayIntersect(orig, dir);
        if (tempIntersect.isIntersecting && tempIntersect.distance < zBuffer) {
            zBuffer = tempIntersect.distance;
            intersect = tempIntersect;
            hitObject = obj;
        }
    }

    if (!intersect.isIntersecting) {  // If no intersection
        return Color(70, 204, 255);  // Sky color
    } 
    // If intersection
    // Calculate the light's direction vector
    glm::vec3 lightDir = glm::normalize(light.position - intersect.point);

    // The dot product of two normalized vectors is the cosine of the angle between them
    float diffuseLightIntensity = std::max(0.0f, glm::dot(intersect.normal, lightDir));

    // Intensity times color for diffuse light (Lambertian reflection)
    Color diffuseLight = light.intensity * diffuseLightIntensity * hitObject->getMaterial().diffuse;

    return diffuseLight;
}



void pixel(glm::vec2 position, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

void render(const Camera& camera, std::vector<Object*>& objects) {
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            // Map the pixel coordinate to screen space [-1, 1]
            float screenX =  (2.0f * x) / SCREEN_WIDTH - 1.0f;
            float screenY = -(2.0f * y) / SCREEN_HEIGHT + 1.0f;

            // Adjust for aspect ratio
            screenX *= ASPECT_RATIO;

            // Calculate the direction of the ray for this pixel
            glm::vec3 rayDirection = glm::normalize(glm::vec3(screenX, screenY, -1.0f));

            // Cast the ray and get the pixel color
            Color pixelColor = castRay(camera.position, rayDirection, objects);

            // Draw the pixel on screen with the returned color
            pixel(glm::vec2(x, y), pixelColor);
        }
    }
}


int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Ray Tracer",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool isRunning = true;
    SDL_Event event;
    float rotateAngle = 0.0f;

    unsigned int lastTime = SDL_GetTicks();
    unsigned int currentTime;
    float dT;

    Material rubber(Color(80, 0, 0));
    Material ivory(Color(100, 100, 80));

    std::vector<Object*> objects;
  
    objects.push_back(
        new Sphere(
            glm::vec3(0.0f, -1.5f, -10.0f),
            1.5f,
            ivory
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(-2.0f, -1.0f, -12.0f),
            2.0f,
            rubber
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(1.0f, 1.0f, -8.0f),
            1.7f,
            rubber
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(0.0f, 5.0f, -20.0f),
            5.0f,
            ivory
        ));

    Camera camera(glm::vec3(0, 0, 0), glm::vec3(-0.25f, 0.875f, -12.5f), 10.0f);

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            // Move closer to the target
                            camera.move(-1.0f);  // You may need to adjust the value as per your needs
                            break;
                        case SDLK_DOWN:
                            // Move away from the target
                            camera.move(1.0f);  // You may need to adjust the value as per your needs
                            break;
                        case SDLK_a:
                            // Rotate up
                            camera.rotate(-1.0f, 0.0f);  // You may need to adjust the value as per your needs
                            break;
                        case SDLK_d:
                            // Rotate down
                            camera.rotate(1.0f, 0.0f);  // You may need to adjust the value as per your needs
                            break;
                        case SDLK_w:
                            // Rotate left
                            camera.rotate(0.0f, -1.0f);  // You may need to adjust the value as per your needs
                            break;
                        case SDLK_s:
                            // Rotate right
                            camera.rotate(0.0f, 1.0f);  // You may need to adjust the value as per your needs
                            break;
                        default:
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Calculate the deltaTime
        currentTime = SDL_GetTicks();
        dT = (currentTime - lastTime) / 1000.0f;  // Time since last frame in seconds
        lastTime = currentTime;

        render(camera, objects);

        SDL_RenderPresent(renderer);
    }

    for (Object* object : objects) {
        delete object;
    }
    objects.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}