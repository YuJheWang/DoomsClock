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
    int w, h;
    SDL_GetMouseState(&w, &h);
    renderer.render({width, height}, float(width) / height, glm::ivec2(w, h));
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

int main(int argc, char* argv[])
{
    DoomsClock app;
    app.loop();
    return 0;
}
