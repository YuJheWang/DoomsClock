#pragma once

#include <iostream>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Render/shader.hpp"

class AppSkeleton
{
public:

    AppSkeleton(const char* title, bool fullscreen = false);

    void loop();

    virtual void render() = 0;

    virtual void keydown(const SDL_Event* event) {}

    virtual void mouseDown(const SDL_Event* event) {}

    ~AppSkeleton();

protected:

    int width, height;

    float wheel;

    struct MouseState
    {
        int prevX, prevY;
        int x = -1, y = -1;
        bool continuePress = false;

        //Call update before you getDelta.
        void getDelta(int* dx, int* dy) 
        { 
            if (!continuePress) { *dx = 0, *dy = 0; return; }  
            *dx = x - prevX; *dy = y - prevY;
            std::cout << *dx << " " << *dy << std::endl;
        }
        void update() 
        {  
            if (!continuePress) 
            {
                SDL_GetMouseState(&prevX, &prevY); 
                SDL_GetMouseState(&x, &y); 
                continuePress = true;
                return;
            }
            prevX = x, prevY = y; 
            SDL_GetMouseState(&x, &y); 
        }
    } mouseState;

private:

    SDL_Window* window = nullptr;

    SDL_GLContext context = nullptr;

    bool running = true, _fullscreen;

    template <class T>
    int check(T obj, const char* msg)
    {
        if (obj == nullptr)
        {
            std::cerr << msg << std::endl;
            SDL_Quit();
            return -1;
        }
        return 0;
    }

};

AppSkeleton::AppSkeleton(const char* title, bool fullscreen) : _fullscreen(fullscreen)
{
    int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL 
        | (SDL_WINDOW_FULLSCREEN * fullscreen)
        | (SDL_WINDOW_RESIZABLE * !fullscreen);
    
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1080 * !fullscreen + 1920 * fullscreen, 720 * !fullscreen + 1080 * fullscreen,
        flags
    );
    check(window, "Fail to create window!");

    context = SDL_GL_CreateContext(window);
    check(context, "Fail to create OpenGL context!");

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        std::cerr << "Fail to load GLAD!" << std::endl;
        SDL_Quit();
    }

    glEnable(GL_DEPTH_TEST);

    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void AppSkeleton::loop()
{
    std::cout << "start loop" << std::endl;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                width = event.window.data1;
                height = event.window.data2;
                glViewport(0, 0, width, height);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (_fullscreen)
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = false;
                keydown(&event);
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                wheel = event.wheel.preciseY;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                mouseDown(&event);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                mouseState.continuePress = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.4f, 0.4f, 1.0f);

        render();

        SDL_GL_SwapWindow(window);
    }
}

AppSkeleton::~AppSkeleton()
{
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);

    SDL_Quit();
}
