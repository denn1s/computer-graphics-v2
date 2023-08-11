#include <array>
#include <algorithm>
#include <glm/glm.hpp>
#include "color.h"  // Include your Color class header
#include "fragment.h"

constexpr size_t SCREEN_WIDTH = 800;
constexpr size_t SCREEN_HEIGHT = 600;

Color backgroundColor{0, 0, 0};

std::array<Color, SCREEN_WIDTH * SCREEN_HEIGHT> framebuffer;
std::array<double, SCREEN_WIDTH * SCREEN_HEIGHT> zbuffer;

// Create a 2D array of mutexes
std::array<std::mutex, SCREEN_WIDTH * SCREEN_HEIGHT> mutexes;

void point(Fragment f) {
    if (f.position.x >= 0 && f.position.x < SCREEN_WIDTH && f.position.y >= 0 && f.position.y < SCREEN_HEIGHT) {
        // Lock the mutex for this pixel
        std::lock_guard<std::mutex> lock(mutexes[f.position.y * SCREEN_WIDTH + f.position.x]);

        if (f.z < zbuffer[f.position.y * SCREEN_WIDTH + f.position.x]) {
            framebuffer[f.position.y * SCREEN_WIDTH + f.position.x] = f.color;
            zbuffer[f.position.y * SCREEN_WIDTH + f.position.x] = f.z;
        }
    }
}

void clearFramebuffer() {
    std::fill(framebuffer.begin(), framebuffer.end(), backgroundColor);
    std::fill(zbuffer.begin(), zbuffer.end(), 99999.0f);
}


void renderBuffer(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    void* texturePixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &texturePixels, &pitch);

    Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

    Uint32* texturePixels32 = static_cast<Uint32*>(texturePixels);
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = y * (pitch / sizeof(Uint32)) + x;
            const Color& color = framebuffer[y * SCREEN_WIDTH + x];
            texturePixels32[index] = SDL_MapRGBA(mappingFormat, color.r, color.g, color.b, color.a);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_Rect textureRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}