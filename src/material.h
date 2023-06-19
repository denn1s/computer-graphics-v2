#pragma once

#include "color.h"
#include <algorithm>

struct Material {
    Color diffuse;
    float albedo;
    float specularAlbedo;
    float specularCoefficient; // The specular coefficient

    Material(const Color& color, float albedo, float specularAlbedo, float specCoef) 
        : diffuse(color),
          albedo(albedo),
          specularAlbedo(specularAlbedo),
          specularCoefficient(specCoef) {}
};
