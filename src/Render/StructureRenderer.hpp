#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cmath>

#include <glm/glm.hpp>

#include "../GameLogic/Structures.hpp"

#define SQRT3 1.7320508f 

class StructureRenderer
{
public:

    StructureRenderer();

    StructureRenderer(SDL_Renderer* renderer);

    ~StructureRenderer();

    void setRenderer(SDL_Renderer* renderer);

    void render(
        const Structure& structure, 
        const char* file, 
        const glm::vec2& pos, const glm::vec2& eyepoint
    );

private:

    SDL_Renderer* _renderer;

    SDL_Texture* _texture = nullptr;

};

StructureRenderer::StructureRenderer() {}

StructureRenderer::StructureRenderer(SDL_Renderer* renderer) : _renderer(renderer) {}

void StructureRenderer::setRenderer(SDL_Renderer* renderer) 
{
    _renderer = renderer;
}

void StructureRenderer::render(
    const Structure& structure,
    const char* file,
    const glm::vec2& pos,
    const glm::vec2& eyePoint
) {
    _texture = IMG_LoadTexture(_renderer, file);
}
