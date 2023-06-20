#include "Skybox.h"
#include <SDL_image.h>

Skybox::Skybox(const std::string& textureFile) {
    loadTexture(textureFile);
}

Skybox::~Skybox() {
    SDL_FreeSurface(texture);
}

void Skybox::loadTexture(const std::string& textureFile) {
    texture = IMG_Load(textureFile.c_str());
    if (!texture) {
        throw std::runtime_error("Failed to load skybox texture: " + std::string(IMG_GetError()));
    }
}

Color Skybox::getColor(const glm::vec3& direction) const {
    // Convert direction vector to spherical coordinates
    float phi = atan2(direction.z, direction.x);
    float theta = acos(direction.y);
    
    // Convert spherical coordinates to texture coordinates
    float u = phi / (2 * M_PI);
    float v = theta / M_PI;
    
    // Map texture coordinates to pixel coordinates
    int x = u * texture->w;
    int y = v * texture->h;
    
    // Get pixel color from texture
    Uint8 r, g, b;
    Uint32 pixel = ((Uint32*)texture->pixels)[y * texture->w + x];
    SDL_GetRGB(pixel, texture->format, &r, &g, &b);

    return Color(r, g, b);
}
