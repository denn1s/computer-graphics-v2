#include <SDL2/SDL.h>

// Define the size of the framebuffer
const int FRAMEBUFFER_WIDTH = 80;
const int FRAMEBUFFER_HEIGHT = 80;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Define a Color struct to hold the RGB values of a pixel
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Declare the framebuffer as a global variable
Color framebuffer[FRAMEBUFFER_SIZE];

// Declare a global clearColor of type Color
Color clearColor = {0, 0, 0, 255}; // Initially set to black

// Declare a global currentColor of type Color
Color currentColor = {255, 255, 255, 255}; // Initially set to white

int x = 10;
int y = 10;

// Function to clear the framebuffer with the clearColor
void clear() {
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        framebuffer[i] = clearColor;
    }
}

// Function to set a specific pixel in the framebuffer to the currentColor
void point(int x, int y) {
    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    // Create a texture
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        FRAMEBUFFER_WIDTH, 
        FRAMEBUFFER_HEIGHT
    );

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
        texture, 
        NULL, 
        framebuffer, 
        FRAMEBUFFER_WIDTH * sizeof(Color)
    );

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

// A temporary framebuffer to hold the next generation
Color nextFramebuffer[FRAMEBUFFER_SIZE];

void life() {
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            int livingNeighbors = 0;

            // Check the 8 neighboring cells
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;  // Skip the cell itself

                    int nx = x + dx;
                    int ny = y + dy;

                    // Check the boundaries
                    if (nx >= 0 && nx < FRAMEBUFFER_WIDTH && ny >= 0 && ny < FRAMEBUFFER_HEIGHT) {
                        if (framebuffer[ny * FRAMEBUFFER_WIDTH + nx].r == 255) {
                            livingNeighbors++;
                        }
                    }
                }
            }

            // Apply the rules of the game
            if (framebuffer[y * FRAMEBUFFER_WIDTH + x].r == 255) {
                if (livingNeighbors < 2 || livingNeighbors > 3) {
                    // Underpopulation or Overpopulation: cell dies
                    nextFramebuffer[y * FRAMEBUFFER_WIDTH + x] = clearColor;
                } else {
                    // Survival: cell stays alive
                    nextFramebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
                }
            } else {
                if (livingNeighbors == 3) {
                    // Reproduction: cell becomes alive
                    nextFramebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
                } else {
                    // Cell stays dead
                    nextFramebuffer[y * FRAMEBUFFER_WIDTH + x] = clearColor;
                }
            }
        }
    }

    // Update the framebuffer for the next frame
    memcpy(framebuffer, nextFramebuffer, FRAMEBUFFER_SIZE * sizeof(Color));
}

// Function to add a glider at a given position
void glider(int x, int y) {
    const int DX[5] = {1, 2, 0, 1, 2};
    const int DY[5] = {0, 0, 1, 1, 2};

    for (int i = 0; i < 5; i++) {
        int nx = x + DX[i];
        int ny = y + DY[i];

        // Check the boundaries
        if (nx >= 0 && nx < FRAMEBUFFER_WIDTH && ny >= 0 && ny < FRAMEBUFFER_HEIGHT) {
            framebuffer[ny * FRAMEBUFFER_WIDTH + nx] = currentColor;
        }
    }
}

void heavySpaceship(int x, int y) {
    const int DX[11] = {0, 4, 0, 1, 2, 3, 4, 5, 1, 2, 5};
    const int DY[11] = {0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2};

    for (int i = 0; i < 11; i++) {
        int nx = x + DX[i];
        int ny = y + DY[i];

        // Check the boundaries
        if (nx >= 0 && nx < FRAMEBUFFER_WIDTH && ny >= 0 && ny < FRAMEBUFFER_HEIGHT) {
            framebuffer[ny * FRAMEBUFFER_WIDTH + nx] = currentColor;
        }
    }
}

void gosperGliderGun(int x, int y) {
    const int DX[36] = {1, 3, 0, 1, 0, 1, 14, 15, 13, 17, 12, 18, 12, 18, 19, 11, 21, 22, 23, 24, 35, 36, 22, 24, 12, 16, 13, 17, 14, 15, 0, 1, 2, 7, 3, 7};
    const int DY[36] = {5, 5, 6, 6, 7, 7, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 4, 4, 5, 5, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 14, 14, 15, 15, 16, 16};

    for (int i = 0; i < 36; i++) {
        int nx = x + DX[i];
        int ny = y + DY[i];

        // Check the boundaries
        if (nx >= 0 && nx < FRAMEBUFFER_WIDTH && ny >= 0 && ny < FRAMEBUFFER_HEIGHT) {
            framebuffer[ny * FRAMEBUFFER_WIDTH + nx] = currentColor;
        }
    }
}

void initialRender() {
    // Clear the framebuffer
    clear();

    // Add some gliders
    // glider(10, 10);
    // glider(20, 20);
    // glider(30, 30);

    // heavySpaceship(40, 40);
    // heavySpaceship(50, 50);

    gosperGliderGun(10, 10);

    // Add more gliders if you want...
}


void render(SDL_Renderer* renderer) {
    life();

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    bool running = true;
    SDL_Event event;

    initialRender();


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Call our render function
        render(renderer);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}