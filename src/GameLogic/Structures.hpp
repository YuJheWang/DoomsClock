#pragma once

#include <string>
#include <sstream>

struct Resources
{
    int log, iron, coal, food;
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
Structure structures[All]; //Store the informations of all kind of structures.

//To initialize the structure informations of all the structures.
void initialize_structures()
{
    structures[Factory] = {
        "工廠",         //name
        {1, 1, 1, 0},   //resourceCost     {log, iron, coal, food}
        0,              //populationGain
        {1, 2},         //size
        3,              //deltaClock
        false,          //earthquakeResist 耐震:true、不耐震:false
        false,          //flamable         易燃:true、不易燃:false
        3,              //electricCost
        ""              //describtion      先空著，我到時候再想
    };
}
