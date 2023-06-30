#include <array>
#include <algorithm>
#include <glm/glm.hpp>
#include "color.h"  // Include your Color class header

constexpr size_t SCREEN_WIDTH = 800;
constexpr size_t SCREEN_HEIGHT = 600;

Color backgroundColor{0, 0, 0};

std::array<std::array<Color, SCREEN_WIDTH>, SCREEN_HEIGHT> framebuffer;

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
            const Color& color = framebuffer[y][x];
            texturePixels32[index] = SDL_MapRGBA(mappingFormat, color.r, color.g, color.b, color.a);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_Rect textureRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void point(int x, int y, Color c) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        framebuffer[y][x] = c; 
    }
}

void point(glm::ivec2 p, Color c) {
    point(p.x, p.y, c);
}

void clearFramebuffer() {
  for (auto &row : framebuffer) {
    std::fill(row.begin(), row.end(), backgroundColor);
  }
}