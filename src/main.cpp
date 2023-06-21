#define FOV glm::radians(90.0f)  // Field of view is 90 degrees
#define ASPECT_RATIO (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT
#define BIAS 0.01f
#define MAX_RECURSION_DEPTH 2
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <execution>

#include "skybox.h"
#include "light.h"
#include "color.h"
#include "object.h"
#include "sphere.h"
#include "camera.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Renderer* renderer = nullptr;
Light light(glm::vec3(20.0f, 0.0f, 0.0f), 1.5f, Color(255, 255, 255));
Camera camera(glm::vec3(-20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
Skybox skybox("./textures/skybox.jpg");

float castShadow(const glm::vec3& shadowOrig, const glm::vec3& lightDir, const std::vector<Object*>& objects, Object* hitObject) {
    for (auto& obj : objects) {
        if (obj != hitObject) {
            Intersect shadowIntersect = obj->rayIntersect(shadowOrig, lightDir);
            if (shadowIntersect.isIntersecting && shadowIntersect.distance > 0) {  // zbuffer?
                const float shadowIntensity =  (1.0f - glm::min(1.0f, shadowIntersect.distance / glm::length(light.position - shadowOrig)));
                return shadowIntensity;
            }
        }
    }

    return 1.0f;
}

Color castRay(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Object*>& objects, const short recursion = 0) {
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

    if (!intersect.isIntersecting || recursion >= MAX_RECURSION_DEPTH) {
        return skybox.getColor(dir);  // Sky color
    }

    const Material& hitMaterial = hitObject->getMaterial();
    // Calculate the light's direction vector
    glm::vec3 lightDir = glm::normalize(light.position - intersect.point);
    
    // Calculate the view direction vector
    glm::vec3 viewDir = glm::normalize(orig - intersect.point);

    // Calculate shadow intensity
    float shadowIntensity = castShadow(
        intersect.point + BIAS * intersect.normal,
        lightDir, objects, hitObject);

    float intensity = shadowIntensity * light.intensity;

    // The dot product of two normalized vectors is the cosine of the angle between them
    float diffuseLightIntensity = std::max(0.0f, glm::dot(intersect.normal, lightDir));

    // Calculate the reflection direction vector
    glm::vec3 reflectDir = glm::reflect(-lightDir, intersect.normal);

    // Calculate the specular light intensity
    float spec = std::pow(std::max(0.0f, glm::dot(viewDir, reflectDir)), hitMaterial.specularCoefficient);

    // Intensity times albedo times color for diffuse light (Lambertian reflection)
    Color diffuseLight = intensity * diffuseLightIntensity * hitMaterial.albedo * hitMaterial.diffuse;

    // Intensity times albedo times color for specular light (Phong reflection)
    Color specularLight = intensity * spec * hitMaterial.specularAlbedo * light.color;

    // If the material is reflective, cast a reflected ray
    Color reflectedColor(0.0f, 0.0f, 0.0f);
    if (hitMaterial.reflectivity > 0) {
        glm::vec3 offsetOrigin = intersect.point + intersect.normal * BIAS; 
        reflectedColor = hitMaterial.reflectivity * castRay(offsetOrigin, reflectDir, objects, recursion + 1);
    }

    // If the material is refractive, cast a refracted ray
    Color refractedColor(0.0f, 0.0f, 0.0f);
    if (hitMaterial.transparency > 0) {        
        glm::vec3 refractDir = glm::refract(dir, intersect.normal, hitMaterial.refractionIndex);
        glm::vec3 offsetOrigin = intersect.point - intersect.normal * BIAS; // moving along opposite to normal for refraction ray
        refractedColor = hitMaterial.transparency * castRay(offsetOrigin, refractDir, objects, recursion + 1);
    }

    // Final color is a mix of local shading (diffuse + specular), reflected color and refracted color
    return (1 - hitMaterial.reflectivity - hitMaterial.transparency) * (diffuseLight + specularLight) + reflectedColor + refractedColor;
}


void pixel(glm::vec2 position, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

void renderFromBuffer(const std::array<std::array<Color, SCREEN_WIDTH>, SCREEN_HEIGHT>& pixels)
{
    int textureWidth = SCREEN_WIDTH;
    int textureHeight = SCREEN_HEIGHT;

    // Create a texture with the desired size and format
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);

    // Lock the texture to gain access to its pixel data
    void* texturePixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &texturePixels, &pitch);


    Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

    // Copy the pixel data from the buffer to the texture pixel data
    Uint32* texturePixels32 = static_cast<Uint32*>(texturePixels);
    for (int y = 0; y < textureHeight; y++) {
        for (int x = 0; x < textureWidth; x++) {
            // Calculate the index in the pixel data corresponding to the current point
            int index = y * (pitch / sizeof(Uint32)) + x;

            // Get the color of the current point from the buffer
            const Color& color = pixels[y][x];

            // Set the color of the point directly in the texture pixel data
            texturePixels32[index] = SDL_MapRGBA(mappingFormat, color.r, color.g, color.b, color.a);
        }
    }

    // Unlock the texture to finalize the changes
    SDL_UnlockTexture(texture);

    // Render the texture
    SDL_Rect textureRect = {0, 0, textureWidth, textureHeight};
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);

    // Destroy the texture when no longer needed
    SDL_DestroyTexture(texture);
}

void render(std::vector<Object*>& objects) {
    // This is the "simulated" up vector
    glm::vec3 simulatedUp = glm::vec3(0, 1, 0);

    std::vector<int> rows(SCREEN_HEIGHT);
    std::iota(rows.begin(), rows.end(), 0);  // Fill it with 0, 1, ..., SCREEN_HEIGHT-1
    std::array<std::array<Color, SCREEN_WIDTH>, SCREEN_HEIGHT> pixels;

    std::for_each(std::execution::par, rows.begin(), rows.end(), [&](int y) {
    // for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            // float random_value = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            // if (random_value > 0.8) {
            // Map the pixel coordinate to screen space [-1, 1]
            float screenX =  (2.0f * x) / SCREEN_WIDTH - 1.0f;
            float screenY = -(2.0f * y) / SCREEN_HEIGHT + 1.0f;

            // Adjust for aspect ratio
            screenX *= ASPECT_RATIO;

            // Compute the direction of the ray for this pixel
            glm::vec3 dir = glm::normalize(camera.target - camera.position);

            // Compute the real right and up vectors
            glm::vec3 right = glm::normalize(glm::cross(dir, simulatedUp));
            glm::vec3 up = glm::cross(right, dir);

            dir = dir + right * screenX + up * screenY;

            // Then cast the ray
            Color pixelColor = castRay(camera.position, glm::normalize(dir), objects);

            // Draw the pixel on screen with the returned color
            // pixel(glm::vec2(x, y), pixelColor);
            // }
            // std::lock_guard<std::mutex> lock(pixelsMutex);
            // pixel(glm::vec2(x, y), pixelColor);
            pixels[y][x] = pixelColor;
        }
    });
    // }
    renderFromBuffer(pixels);
}

void handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            // Move closer to the target
            camera.move(-1.0f); // You may need to adjust the value as per your needs
            break;
        case SDLK_DOWN:
            // Move away from the target
            camera.move(1.0f); // You may need to adjust the value as per your needs
            break;
        case SDLK_a:
            // Rotate up
            camera.rotate(-1.0f, 0.0f); // You may need to adjust the value as per your needs
            break;
        case SDLK_d:
            // Rotate down
            camera.rotate(1.0f, 0.0f); // You may need to adjust the value as per your needs
            break;
        case SDLK_w:
            // Rotate left
            camera.rotate(0.0f, -1.0f); // You may need to adjust the value as per your needs
            break;
        case SDLK_s:
            // Rotate right
            camera.rotate(0.0f, 1.0f); // You may need to adjust the value as per your needs
            break;
        default:
            break;
    }
}

void processKeyEvents(const SDL_Event& event, std::unordered_map<SDL_Keycode, bool>& keyStates) {
    switch (event.type) {
        case SDL_KEYDOWN:
            handleKeyPress(event.key.keysym.sym);
            keyStates[event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            keyStates[event.key.keysym.sym] = false;
            break;
        default:
            break;
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

    Material ivory(
        Color(100, 100, 80),
        0.6f,
        0.3f,
        50.0f,
        0.2f
    );
    Material rubber(
        Color(80, 0, 0),
        0.9f,
        0.1f,
        10.0f,
        0.0f
    );
    Material mirror(
        Color(255, 255, 255),
        0.0f,
        10.0f,
        1425.0f,
        0.9f
    );
    Material glass(
        Color(255, 255, 255),
        0.1f,
        1.0f,
        125.0f,
        0.0f,
        0.9f,
        0.1f
    );

    std::vector<Object*> objects;
    
    objects.push_back(
        new Sphere(
            glm::vec3(2.0f, 4.5f, -2.0f),
            1.1f,
            rubber
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(3.0f, 4.0f, 2.0f),
            1.7f,
            ivory
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(-1.3f, 3.0f, -3.0f),
            3.0f,
            rubber
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(0.0f, 0.0f, 3.0f),
            2.0f,
            mirror
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(0.0f, 0.0f, -9.0f),
            3.0f,
            glass
        ));
    
    /*
    objects.push_back(
        new Sphere(
            glm::vec3(-3.0f, 0.0f, 0.0f),
            3.0f,
            glass
        ));
    objects.push_back(
        new Sphere(
            glm::vec3(3.0f, 0.0f, 0.0f),
            1.1f,
            rubber
        ));
    */
    int frameCount = 0;
    float elapsedTime = 0.0f;

    std::unordered_map<SDL_Keycode, bool> keyStates;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                default:
                    processKeyEvents(event, keyStates);
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render(objects);

        SDL_RenderPresent(renderer);

        // Calculate the deltaTime
        currentTime = SDL_GetTicks();
        dT = (currentTime - lastTime) / 1000.0f;  // Time since last frame in seconds
        lastTime = currentTime;

        frameCount++;
        elapsedTime += dT;
        if (elapsedTime >= 1.0f) {
            float fps = static_cast<float>(frameCount) / elapsedTime;
            std::cout << "FPS: " << fps << std::endl;

            frameCount = 0;
            elapsedTime = 0.0f;
        }
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