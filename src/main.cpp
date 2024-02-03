#include <SDL2/SDL.h>

#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* rootRender = nullptr;
SDL_Surface* root = nullptr;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "Dooms Clock", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1920, 1080,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
    ); 
    if (window == nullptr) 
    {
        std::cerr << "Fail to create window!" << std::endl;
        SDL_Quit();
        return -1;
    }

    root = SDL_GetWindowSurface(window);
    if (root == nullptr) 
    {
        std::cerr << "Fail to create root surface!" << std::endl;
        SDL_Quit();
        return -1;
    }

    rootRender = SDL_CreateSoftwareRenderer(root);
    if (rootRender == nullptr)
    {
        std::cerr << "Fail to create root renderer!" << std::endl;
        SDL_Quit();
        return -1;
    }

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
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
