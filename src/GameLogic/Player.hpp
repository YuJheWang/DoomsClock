#include <string>
#include <vector>
#include <map>
#include "Structures.hpp"

#include <glm/glm.hpp>

#define POSITION first
#define STRUCTURE_INDEX second

class Player
{
public:

    Player();

    int getTotalElectricCost();

    int getTotalDeltaClock();

    static Player* createTestPlayer();

    std::vector<std::pair<glm::vec2, unsigned int>> getContainedStructure() 
    { return structureContained; }

    void addStructure(const std::pair<glm::vec2, unsigned int>& newstruct) 
    { structureContained.push_back(newstruct); }

private:

    std::vector<std::pair<glm::vec2, unsigned int>> structureContained;

    std::vector<glm::vec2> territory;

};

Player::Player() 
{
    
}

Player* Player::createTestPlayer()
{           
    Player* player = new Player;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            player->territory.push_back(glm::vec2(i, j));
    
    player->structureContained = {
        { glm::vec2(0, 0), NuclearPowerPlant }
    };
    return player;
}

int Player::getTotalElectricCost()
{

}

int Player::getTotalDeltaClock()
{

}
