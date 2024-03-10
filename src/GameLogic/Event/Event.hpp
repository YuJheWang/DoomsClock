#pragma once

#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include "../Room.hpp"
#include "../Player.hpp"

class Event 
{
public:

    friend class Player;

    Event(int f, int d, int t, const std::string& n) : 
        effectFrequency(f), effectDuration(d), effectTimes(t), name(n) {}

    void startEvent();
    
    void detect();

    void bindPlayer(Player* player) { _player = player; }

    virtual void effect(Player* player, const glm::vec2& pos, int effectCount) = 0;

protected:

    Player* _player;

    int effectFrequency;

    int effectDuration;

    int effectTimes;

    std::string name;

private:

    int startTime;

};

void Event::startEvent()
{
    startTime = SDL_GetTicks64();
}

void Event::detect()
{
    int currentTime = SDL_GetTicks64();
    int timePass = currentTime - startTime;
    if (timePass >= effectDuration * 1000)
    {
        srand(time(NULL));
        int x = rand() % 27;
        srand(time(NULL));
        int y = rand() % 27;
        effect(_player, { x, y }, 0);
    }
}

class Wildfire : public Event
{
public:

    friend class Player;

    Wildfire() : Event(0, 5, 3, "Wildfire") {}

    virtual void effect(Player* player, const glm::vec2& pos, int effectCount)
    {
        std::set<glm::vec2> affectPositions;
        glm::vec2 pn[] = {
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, -1.0f),
            glm::vec2(0.0f, -1.0f),
            glm::vec2(-1.0f, -1.0f),
            glm::vec2(-1.0f, 0.0f),
            glm::vec2(-1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f)
        };
        for (auto r : pn)
        {
            glm::vec2 position = pos + r * float(effectCount);
            position = glm::vec2(
                std::max(std::min(position.x, 26.0f), 0.0f),
                std::max(std::min(position.y, 26.0f), 0.0f)
            );
            affectPositions.emplace(position);
        } 

        
    }

};
