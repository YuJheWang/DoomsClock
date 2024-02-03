#include <string>
#include <vector>
#include "Structures.hpp"

template <size_t size>
class City
{
public:

    City();

    int getTotalElectricCost();

    int getTotalDeltaClock();

private:

    unsigned int contents[size];

    std::vector<unsigned int> structureContained;

};
