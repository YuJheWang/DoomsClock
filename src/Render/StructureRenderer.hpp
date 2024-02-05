#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cmath>

#include "../GameLogic/Structures.hpp"

#define SQRT3 1.7320508f 

float unitVertices[14] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    -SQRT3 / 2, 0.5f,
    -SQRT3 / 2, -0.5f,
    0.0F, -1.0f,
    SQRT3 / 2, -0.5f,
    SQRT3 / 2, 0.5f
};

class StructureRenderer
{
public:

    StructureRenderer();

    ~StructureRenderer();

    void render(const Structure& structure, const char* file);

private:

};

StructureRenderer::StructureRenderer()
{

}

void StructureRenderer::render(const Structure& structure, const char* file)
{
    int x = structure.size.x, y = structure.size.y;
}
