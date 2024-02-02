#pragma once

#include <string>
#include <sstream>

struct Resources
{
    int log, iron, coal, food;
    std::string getDescribtionText();
};

struct Size { unsigned int width, height; };

struct Describtion
{
    std::string name;
    std::string text;
};

struct Structure
{

    std::string name;

    Resources resourceCost;

    int populationGain;

    Size size;

    int deltaClock;

    bool earthquakeResist;

    bool flamable;

    unsigned int electricCost;
    
    int populationGain;

    std::string describtionText;

};

enum StructureIndex { 
    Factory,
    FirePowerPlant,
    NuclearPowerPlant,
    FireDepartment,
    Park,
    Coal,
    Farm,
    Storage,
    All
};
Structure structures[All];
