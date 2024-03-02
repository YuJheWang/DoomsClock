#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#include "AppSkeleton.hpp"
//#include "UI/UISystem.hpp"
#include "Render/RoomRender.hpp"
#include "UI/StructureUI.hpp"

class DoomsClock : public AppSkeleton
{
public:

    DoomsClock();

    virtual void render();

    virtual void keydown(const SDL_Event* event);

    virtual void mouseDown(const SDL_Event* event);

    virtual void mouseUp(const SDL_Event* event);

    virtual void mouseMotion(const SDL_Event* event);

    virtual void mouseWheel(const SDL_Event* event);

private:

    RoomRender renderer;

    Room* currentRoom = nullptr;

    glm::vec4 currentPos;

    StructureUI* su;

    StructureImageRender* sir;

    enum UIFlagIndex { StructureUIIndex };
    bool flags[4] = { true };

    float zoom = 1.0f;

};

DoomsClock::DoomsClock() : AppSkeleton("Dooms Clock", true)
{
    currentRoom = Room::createTestRoom();
    renderer.bind(currentRoom);
    su = new StructureUI(window, context);
    su->loadImGui();

    sir = new StructureImageRender;
}

void DoomsClock::render()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    /*if (su->getCurrentBindStructure() != All)
    {
        
    }*/
    renderer.render({width, height}, float(width) / height, glm::ivec2(x, y), zoom);
    su->renderToWindow(flags + StructureUIIndex);
    glDisable(GL_DEPTH_TEST);
    sir->render(su->getCurrentBindStructure(), {x, y}, {width, height});
    glEnable(GL_DEPTH_TEST);
}

void DoomsClock::keydown(const SDL_Event* event)
{
    /*switch (event->key.keysym.sym)
    {
    case SDLK_w: renderer.lookPoint += glm::vec3(1, 0, 1) * 0.1f; break;
    case SDLK_a: renderer.lookPoint -= glm::vec3(-1, 0, 1) * 0.1f; break;
    case SDLK_s: renderer.lookPoint -= glm::vec3(1, 0, 1) * 0.1f; break;
    case SDLK_d: renderer.lookPoint -= glm::vec3(1, 0, -1) * 0.1f; break;
    }*/
}

void DoomsClock::mouseDown(const SDL_Event* event)
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {
        mouseState.update();

        auto getMousePos = [&](int x, int y) 
        {
            glm::vec2 normalizedMousePos = 
                glm::vec2(x / float(width), y / float(height)) * 2.0f - 1.0f;
            normalizedMousePos.y = -normalizedMousePos.y;
            glm::mat4 projMat = glm::ortho(
                -8.0f * zoom, 8.0f * zoom, 
                -4.5f * zoom, 4.5f * zoom, 
                0.01f, 100.0f
            );

            glm::vec4 mousePos = 
                glm::inverse(projMat * renderer.getCamViewMat()) *
                glm::vec4(normalizedMousePos, 0.0f, 1.0f);
                float k = sqrt(2) * mousePos.y;
                mousePos += glm::vec4(0.5f, -sqrt(2) / 2.0f, 0.5f, 0.0f) * k;

            return mousePos;
        };

        glm::vec4 prevPos = getMousePos(mouseState.prevX, mouseState.prevY);
        currentPos = getMousePos(mouseState.x, mouseState.y);
        glm::vec3 delta = currentPos - prevPos;

        if (!su->isWindowHovered())
        {
            renderer.lookPoint -= delta;
            mouseState.activatedButton = SDL_BUTTON_LEFT;
        }
    }
    else if (event->button.button == SDL_BUTTON_RIGHT)
    {
        mouseState.activatedButton = SDL_BUTTON_RIGHT;
        su->getCurrentBindStructure() = All;
    }
}

void DoomsClock::mouseUp(const SDL_Event* event)
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {
        mouseState.continuePress = false;
        mouseState.activatedButton = 0;
        if (su->getCurrentBindStructure() != All)
        {
            if (currentPos.x < 0 || currentPos.z < 0 || currentPos.x > 27 || currentPos.y > 27)
                su->getCurrentBindStructure() = All;
            auto& structure = su->getCurrentBindStructure();
            currentRoom->getPlayers()[0]->addStructure({{currentPos.x, currentPos.z}, structure});
            structure = All;
        }
    }
    else if (event->button.button == SDL_BUTTON_RIGHT)
    {
        mouseState.activatedButton = 0;
    }
}

void DoomsClock::mouseMotion(const SDL_Event* event)
{
    auto getMousePos = [&](int x, int y) 
    {
        glm::vec2 normalizedMousePos = 
            glm::vec2(x / float(width), y / float(height)) * 2.0f - 1.0f;
        normalizedMousePos.y = -normalizedMousePos.y;
        glm::mat4 projMat = glm::ortho(
            -8.0f * zoom, 8.0f * zoom, 
            -4.5f * zoom, 4.5f * zoom, 
            0.01f, 100.0f
        );

        glm::vec4 mousePos = 
            glm::inverse(projMat * renderer.getCamViewMat()) *
            glm::vec4(normalizedMousePos, 0.0f, 1.0f);
            float k = sqrt(2) * mousePos.y;
            mousePos += glm::vec4(0.5f, -sqrt(2) / 2.0f, 0.5f, 0.0f) * k;

        return mousePos;
    };

    int x, y;
    SDL_GetMouseState(&x, &y);
    currentPos = getMousePos(mouseState.x, mouseState.y);

    if (mouseState.activatedButton == SDL_BUTTON_LEFT)
    {
        mouseState.update();

        glm::vec4 prevPos = getMousePos(mouseState.prevX, mouseState.prevY);
        currentPos = getMousePos(mouseState.x, mouseState.y);
        glm::vec3 delta = currentPos - prevPos;

        //std::cout << delta.x << " " << delta.z << std::endl;

        renderer.lookPoint -= delta;
    }
}

void DoomsClock::mouseWheel(const SDL_Event* event)
{
    float delta = event->wheel.preciseY;
    zoom -= delta * 0.07f;
    zoom = std::max(zoom, 0.7f);
    zoom = std::min(zoom, 2.0f);
}

int main(int argc, char* argv[])
{
    DoomsClock app;
    app.loop();
    return 0;
}
