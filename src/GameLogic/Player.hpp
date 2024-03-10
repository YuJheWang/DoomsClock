#pragma once

enum { DestroyStructure, CutTree, ExtinguishFire, PlantTree, AllM };

enum { WildFire, RainStorm, SevereCold, Drought, Tsunami, Earthquake };

#include <string>
#include <vector>
#include <map>
#include <random>
#include "Structures.hpp"
//#include "Event/Event.hpp"

#include <glm/glm.hpp>

#define POSITION first
#define STRUCTURE_INDEX second

struct Time
{
    int hour;
    int minute;
    int second;

    int getSecond() const { return (hour * 3600 + minute * 60 + second); }
};

Time operator+(const Time& left, const Time& right)
{
    int totalSecond = left.getSecond() + right.getSecond();
    return (Time{totalSecond / 3600, totalSecond / 60, totalSecond % 60});
}

bool operator>(const Time& left, const Time& right)
{
    return (left.getSecond() > right.getSecond());
}

class RoomRender;

class Event;

glm::ivec2 operator+(const glm::ivec2& left, const Size& size)
{
    return (glm::ivec2{left.x + size.x, left.y + size.y});
}

typedef std::vector<std::pair<glm::vec2, unsigned int>>::iterator Iter;

struct Card
{
    Uint8 index;
    int count;
};
int randint(int n)
{
    int end = RAND_MAX / n;
    end *= n;
    std::random_device rd;
    std::mt19937 random(rd());
    int r;
    r = random();
    while ((r = random()) > end);
    return abs(r % n);
}

class Player
{
public:

    enum NegativeStatus { 
        NoElectric = 1 << 0, 
        Chill = 1 << 1, 
        Flood = 1 << 2, 
        NoFoodProduce = 1 << 3, 
        NoProductivity = NoElectric | NoFoodProduce
    };

    friend class RoomRender;

    friend class Event;

    friend class Wildfire;

    Player();

    int getTotalElectricCost();

    int getTotalDeltaClock();

    void addStructureCard();

    void addMeasureCard();

    void useStructureCard(int structure, const glm::ivec2& position);

    void useMeasureCard(int measure, const glm::ivec2& position);

    void viewDecks()
    {
        std::cout << "Structure Decks: " << std::endl;
        for (const auto& s : structureCardDeck) 
            std::cout << "    ID: " << int(s.index) << ", Count: " << s.count << std::endl;
        std::cout << "Measure Decks: " << std::endl;
        for (const auto& m : measureCardDeck) 
            std::cout << "    ID: " << int(m.index) << ", Count: " << m.count << std::endl;
    }

    int getStructureCardCount(int i)
    {
        return structureCardDeck[i].count;
    }

    std::vector<Uint8> getStructureCardIndices()
    {
        std::vector<Uint8> indices(structureCardDeck.size());
        for (int i = 0; i < structureCardDeck.size(); i++) indices[i] = structureCardDeck[i].index;
        return indices;
    }

    int getStructureCardTypeCount() 
    { 
        return structureCardDeck.size();
    }

    int setStructureCardCount(int structure, int count);

    int getMeasureCardCount(int i)
    {
        return measureCardDeck[i].count;
    }

    std::vector<Uint8> getMeasureCardIndices()
    {
        std::vector<Uint8> indices(measureCardDeck.size());
        for (int i = 0; i < measureCardDeck.size(); i++) indices[i] = measureCardDeck[i].index;
        return indices;
    }

    int getMeasureCardTypeCount() 
    { 
        return measureCardDeck.size();
    }

    static Player* createInitPlayer();

    std::vector<std::pair<glm::vec2, unsigned int>> getContainedStructure()
    { return structureContained; }

    void addStructure(const std::pair<glm::vec2, unsigned int>& newstruct);

    void deleteStructure(const glm::ivec2& spos);

    void applyStatus(int status) { negativeStatus |= status; }

    void applyEvent(int event, const glm::ivec2& postion);

private:

    Time clock = {};

    std::vector<std::pair<glm::vec2, unsigned int>> structureContained;

    std::vector<glm::vec2> territory;

    std::vector<glm::vec2> burningRegion;

    Uint8 grid[27][27] = {{All}};

    int negativeStatus = 0;

    std::vector<Card> structureCardDeck, measureCardDeck;

    glm::vec2 initLookAtPos;

    Resources resourcesYouHave = { 0, 0, 0, 0 };

    bool isAnyNeighbor(const glm::ivec2& pos, const Size& size);

    bool isOccupied(const glm::ivec2& pos, const Size& size);

    Iter getStructureThisGrid(const glm::ivec2& position);

};

void Player::addStructureCard()
{
    Uint8 newCard = randint(All);

    for (auto& c : structureCardDeck)
    {
        if (c.index == newCard)
        {
            c.count++;
            return;
        }
    }
    structureCardDeck.push_back({ newCard, 1 });
}

void Player::addMeasureCard()
{
    Uint8 newCard = randint(AllM);

    for (auto& c : measureCardDeck)
    {
        if (c.index == newCard)
        {
            c.count++;
            return;
        }
    }
    measureCardDeck.push_back({ newCard, 1 });
}

Iter Player::getStructureThisGrid(const glm::ivec2& position)
{
    auto structure = structures[grid[(int)position.y][(int)position.x]];
    auto size = structure.size;
    for (auto s = structureContained.begin(); s != structureContained.end(); s++)
    {
        if (size == Size{ 1, 1 })
            if (*s == std::pair<glm::vec2, unsigned int>({position, grid[(int)position.y][(int)position.x]}))
                return s;
        else if (size == Size{ 2, 1 })
            if (*s == std::pair<glm::vec2, unsigned int>({position, grid[(int)position.y][(int)position.x]}))
                return s;
            else if (*s == std::pair<glm::vec2, unsigned int>({position, grid[(int)position.y][(int)position.x - 1]}))
                return s;
        else if (size == Size{ 2, 2 })
            if (*s == std::pair<glm::vec2, unsigned int>({position, grid[(int)position.y][(int)position.x]}))
                return s;
            else if (*s == std::pair<glm::vec2, unsigned int>({position, grid[(int)position.y - 1][(int)position.x - 1]}))
                return s;
            else if (*s == std::pair<glm::vec2, unsigned int>({position, grid[(int)position.y][(int)position.x - 1]}))
                return s;
            else if (*s == std::pair<glm::vec2, unsigned int>({position, grid[(int)position.y - 1][(int)position.x]}))
                return s;
    }
}

void Player::addStructure(const std::pair<glm::vec2, unsigned int>& newstruct)
{
    glm::ivec2 pos = newstruct.first;
    glm::ivec2 leftTop = newstruct.first + structures[newstruct.second].size;
    if (leftTop.x - 1 >= 27 || leftTop.y - 1 >= 27 || pos.x < 0 || pos.y < 0)
        return;
    if (
        isAnyNeighbor(newstruct.first, structures[newstruct.second].size) 
        && !isOccupied(newstruct.first, structures[newstruct.second].size)
    ) {
        structureContained.push_back(newstruct);
        for (int i = pos.x; i < leftTop.x; i++)
            for (int j = pos.y; j < leftTop.y; j++)
                grid[j][i] = newstruct.second;
    }
}

void Player::deleteStructure(const glm::ivec2& spos)
{
    auto s = getStructureThisGrid(spos);
    structureContained.erase(s);
    auto size = structures[(*s).second].size;
    auto pos = (*s).first;
    auto leftTop = pos + size;
    for (int i = pos.x; i < leftTop.x; i++)
        for (int j = pos.y; j < leftTop.y; j++) 
            grid[j][i] = All;
}

bool Player::isAnyNeighbor(const glm::ivec2& pos, const Size& size)
{
    bool result = false;
    auto leftTop = pos + size;
    for (int i = pos.x; i < leftTop.x; i++)
    {
        for (int j = pos.y; j < leftTop.y; j++)
        {
            if (j - 1 >= 0) result = (grid[j - 1][i] != All) || result;
            if (j + 1 < 27) result = (grid[j + 1][i] != All) || result;
            if (i - 1 >= 0) result = (grid[j][i - 1] != All) || result;
            if (i + 1 < 27) result = (grid[j][i + 1] != All) || result;
        }
    }
    return result;
}

bool Player::isOccupied(const glm::ivec2& pos, const Size& size)
{
    bool result = false;
    auto leftTop = pos + size;
    for (int i = pos.x; i < leftTop.x; i++)
        for (int j = pos.y; j < leftTop.y; j++)
            result = result || (grid[j][i] != All);
    return result;
}

Player::Player() 
{
    for (auto& s : structureCardDeck) s = {All, 0};
    for (auto& m : measureCardDeck) m = {AllM, 0}; 
}

Player* Player::createInitPlayer()
{           
    Player* player = new Player;

    player->initLookAtPos = glm::vec2(3.0f, 3.0f);

    for (int i = 0; i < 27; i++)
        for (int j = 0; j < 27; j++)
            player->grid[i][j] = All;
    
    std::pair<glm::vec2, unsigned int> newStructure =  {glm::vec2(0, 0), FirePowerPlant};
    player->structureContained.push_back(newStructure);
    player->grid[0][0] = FirePowerPlant;
    player->grid[0][1] = FirePowerPlant;
    player->grid[1][1] = FirePowerPlant;
    player->grid[1][0] = FirePowerPlant;
    return player;
}

int Player::getTotalElectricCost()
{

}

int Player::getTotalDeltaClock()
{

}

void Player::useStructureCard(int structure, const glm::ivec2& position)
{
    if (
        isAnyNeighbor(position, structures[structure].size) 
        && !isOccupied(position, structures[structure].size)
    ) {
        for (auto iter = structureCardDeck.begin(); iter != structureCardDeck.end(); iter++)
        {
            if (iter->index == structure)
            {
                iter->count--;
                if (iter->count <= 0) structureCardDeck.erase(iter);
                break;
            }
        }   
    }
}

void Player::useMeasureCard(int measure, const glm::ivec2& position)
{
    if (measure == DestroyStructure)
    {
        if (isOccupied(position, {1, 1}))
        {
            for (auto iter = measureCardDeck.begin(); iter != measureCardDeck.end(); iter++)
                if (iter->index == measure)
                {
                    iter->count--;
                    if (iter->count <= 0) measureCardDeck.erase(iter);
                }
            deleteStructure(position);
        }
    }
    else if (measure == ExtinguishFire)
    {
        for (auto iter = measureCardDeck.begin(); iter != measureCardDeck.end(); iter++)
            if (iter->index == measure)
            {
                iter->count--;
                if (iter->count <= 0) measureCardDeck.erase(iter);
            }
        burningRegion.clear();
    }
    else if (measure == CutTree)
    {
        if (grid[int(position.y)][int(position.x)] == Tree)
            grid[int(position.y)][int(position.x)] = All;
    }
    else if (measure == PlantTree)
    {
        if (grid[int(position.y)][int(position.x)] == All)
            grid[int(position.y)][int(position.x)] = Tree;
    }
}

void Player::applyEvent(int event, const glm::ivec2& position)
{
    //WildFire, RainStorm, SevereCold, Drought, Tsunami, Earthquake
    switch (event)
    {
    case WildFire:
        burningRegion.push_back(position);
    case RainStorm:
        applyStatus(NoElectric);
        if (randint(2)) applyStatus(Flood);
    case SevereCold:
        applyStatus(NoProductivity);
    case Drought:
        applyStatus(NoFoodProduce);
    case Tsunami:
        if (clock > Time{24, 0, 0} + Time{0, 0, -100})
            for (auto structure : structureContained)
                if (structure.second == NuclearPowerPlant)
                    deleteStructure(structure.first);
    case Earthquake:
        for (auto structure: structureContained)
            if (!structures[structure.second].earthquakeResist)
                deleteStructure(structure.first);
    }
}
