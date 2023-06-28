#include <SDL2/SDL.h>
#include <iostream>
#include "color.h"
#include "framebuffer.h"

Framebuffer framebuffer;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

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

void renderBuffer(int textureWidth, int textureHeight) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);

    void* texturePixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &texturePixels, &pitch);

    Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

    Uint32* texturePixels32 = static_cast<Uint32*>(texturePixels);
    for (int y = 0; y < textureHeight; y++) {
        for (int x = 0; x < textureWidth; x++) {
            int index = y * (pitch / sizeof(Uint32)) + x;
            const Color& color = framebuffer[y][x];
            texturePixels32[index] = SDL_MapRGBA(mappingFormat, color.r, color.g, color.b, color.a);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_Rect textureRect = {0, 0, textureWidth, textureHeight};
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (!init()) {
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        point(400, 300, Color(255, 0, 0));

        renderBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}