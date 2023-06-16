#pragma once

#include "color.h"

struct Material {
    Color diffuse;

    Material(const Color& color) : diffuse(color) {}
};