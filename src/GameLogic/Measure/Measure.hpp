#pragma once

#include "../Player.hpp"

class Measure
{
public:

    Measure() =default;

    virtual void effect(Player* player) = 0;

protected:

private:
};
