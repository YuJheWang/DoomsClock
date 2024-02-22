#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#include "AppSkeleton.hpp"
//#include "UI/UISystem.hpp"
#include "Render/RoomRender.hpp"

class DoomsClock : public AppSkeleton
{
public:

    DoomsClock();

    virtual void render();

    virtual void keydown(const SDL_Event* event);

    virtual void mouseDown(const SDL_Event* event);

private:

    RoomRender renderer;

    Room* currentRoom = nullptr;

};

DoomsClock::DoomsClock() : AppSkeleton("Dooms Clock", true)
{
    currentRoom = Room::createTestRoom();
    renderer.bind(currentRoom);
}

void DoomsClock::render()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    renderer.render({width, height}, float(width) / height, glm::ivec2(x, y));
}

void DoomsClock::keydown(const SDL_Event* event)
{
    switch (event->key.keysym.sym)
    {
    case SDLK_w: renderer.lookPoint += glm::vec3(1, 0, 1) * 0.1f; break;
    case SDLK_a: renderer.lookPoint -= glm::vec3(-1, 0, 1) * 0.1f; break;
    case SDLK_s: renderer.lookPoint -= glm::vec3(1, 0, 1) * 0.1f; break;
    case SDLK_d: renderer.lookPoint -= glm::vec3(1, 0, -1) * 0.1f; break;
    }
}

void DoomsClock::mouseDown(const SDL_Event* event)
{
    int dx, dy;
    if (event->button.button == SDL_BUTTON_RIGHT)
    {
        auto getMousePos = [&](int x, int y) 
        {
            glm::vec2 normalizedMousePos = 
                glm::vec2(dx / float(width), dy / float(height)) * 2.0f - 1.0f;
            normalizedMousePos.y = -normalizedMousePos.y;
            glm::mat4 projMat = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, 0.01f, 100.0f);

            glm::vec4 mousePos = 
                glm::inverse(projMat * renderer.getCamViewMat()) *
                glm::vec4(normalizedMousePos, 0.0f, 1.0f);
                float k = sqrt(2) * mousePos.y;
                mousePos += glm::vec4(0.5f, -sqrt(2) / 2.0f, 0.5f, 0.0f) * k;

            return mousePos;
        }
        mouseState.update();

        glm::vec4 prevPos = getMousePos(mouseState.prevX, mouseState.prevY);
        glm::vec4 currentPos = getMousePos(mouseState.x, mouseState.y);
        glm::vec4 delta = currentPos - prevPos;

        renderer.lookPoint += delta;
    }
}

int main(int argc, char* argv[])
{
    DoomsClock app;
    app.loop();
    return 0;
}
