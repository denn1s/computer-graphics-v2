#pragma once

#include "color.h"
#include <algorithm>

struct Material {
    Color diffuse;
    float albedo;
    float specularAlbedo;
    float specularCoefficient; // The specular coefficient
    float reflectivity; // The reflectivity of the material

    Material(const Color& color, float albedo, float specularAlbedo, float specCoef, float reflectivity) 
        : diffuse(color),
          albedo(albedo),
          specularAlbedo(specularAlbedo),
          specularCoefficient(specCoef),
          reflectivity(reflectivity) {}
};