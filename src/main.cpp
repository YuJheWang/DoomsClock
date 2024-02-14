#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#include "UI/UISystem.hpp"
#include "Render/SceneRender.hpp"

SDL_Window* window = nullptr;
SDL_GLContext context = nullptr;

SceneRender sceneRender;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "Dooms Clock", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1920, 1080,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN |SDL_WINDOW_OPENGL
    );
    if (window == nullptr)
    {
        std::cerr << "Fail to create window!" << std::endl;
        SDL_Quit();
        return -1;
    }

    context = SDL_GL_CreateContext(window);
    if (context == nullptr)
    {
        std::cerr << "Fail to create OpenGL Context!" << std::endl;
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        std::cerr << "Fail to initialize GLAD!" << std::endl;
        SDL_Quit();
        return -1;
    }

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    UISystem ui(window, context);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYUP) 
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                    running = false;
            }
        }

        //sceneRender.render();

        ui.loop(event);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
