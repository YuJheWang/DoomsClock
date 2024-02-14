#include <string>
#include <vector>
#include <map>
#include "Structures.hpp"

#include <glm/glm.hpp>

#define POSITION first
#define STRUCTURE_INDEX second

template <size_t size>
class City
{
public:

    City();

    int getTotalElectricCost();

    int getTotalDeltaClock();

private:

    unsigned int contents[size];

    std::vector<std::pair<glm::vec2, unsigned int>> structureContained;

};
