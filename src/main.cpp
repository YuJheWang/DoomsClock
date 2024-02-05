#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#include "UI/UISystem.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    UISystem ui(window, renderer);

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

        SDL_RenderClear(renderer);

        ui.loop(event);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
