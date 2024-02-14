#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

#include <iostream>
#include <string>

class SceneRender
{
public:

    SceneRender();

    void render();

private:

    int gridSide = 50;

    void renderSingleGrid(const glm::ivec2& center);

};

SceneRender::SceneRender() {}

void SceneRender::render()
{
    
}

void SceneRender::renderSingleGrid(const glm::ivec2& center)
{
    
}
