#include <array>
#include "color.h"  // Include your Color class header

constexpr size_t SCREEN_WIDTH = 800;
constexpr size_t SCREEN_HEIGHT = 600;

// Create a framebuffer alias using the Color struct and the defined screen dimensions
using Framebuffer = std::array<std::array<Color, SCREEN_WIDTH>, SCREEN_HEIGHT>;

