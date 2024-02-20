#pragma once

#include <SDL2/SDL.h>

class Timer
{
public:

    Timer(int time) : _activeTime(time) {}

    void setActiveTime(int time) { _activeTime = time; }

    void start() { paused = false; startTime = SDL_GetTicks64(); }

    void pause() { paused = true; }

    void restart() { paused = false; startTime = SDL_GetTicks64(); }

    void stop() { paused = true; totalPassTime = 0; }

    void updatePerFrame();

    int getTicks() { return totalPassTime; }

    virtual void action() = 0;

private:

    bool paused = true;

    Uint64 startTime;

    int totalPassTime = 0;

    int _activeTime;

};

void Timer::updatePerFrame()
{
    if (!paused)
    {
        int currentTime = SDL_GetTicks64();
        totalPassTime += currentTime - startTime;
        if (totalPassTime >= _activeTime) 
        {
            action();
            totalPassTime = 0;
            paused = true;
            startTime = 0;
        }
    }
}
