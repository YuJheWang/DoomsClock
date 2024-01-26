#include <SDL2/SDL.h>
#include <iostream>

SDL_Window* window = nullptr;

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "Dooms clock", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1080, 720,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_SHOWN
    );
    if (window == nullptr)
    {
        std::cerr << "Fail to create window!" << std::endl;
        SDL_Quit();
    }
}
