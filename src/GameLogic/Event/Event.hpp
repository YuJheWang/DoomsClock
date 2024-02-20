#pragma once

#include <bits/stdc++.h>
#include "../Room.hpp"

class Event 
{
public:

    Event() = default;

    virtual void effect(Room *room) = 0;

private:
};
