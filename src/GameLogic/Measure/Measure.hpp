#pragma once

#include "../Player.hpp"

class Measure
{
public:

    Measure() =default;

    virtual void effect(Player* player, const glm::vec2& pos) = 0;

protected:

private:
};

class DestroyStructure : public Measure
{
public:

    DestroyStructure() = default;

    virtual void effect(Player* player, const glm::vec2& pos);

private:

};

void DestroyStructure::effect(Player* player, const glm::vec2& pos)
{
    player->deleteStructure(pos);
}

class CutTree : public Measure
{
public:

    virtual void effect(Player* player, const glm::vec2& pos);

private:
};

void CutTree::effect(Player* player, const glm::vec2& pos)
{
    
}
