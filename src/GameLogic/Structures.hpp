#pragma once

#include <string>
#include <sstream>

struct Resources { int log, iron, coal, food; };

struct Size { unsigned int x, y; };

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

    std::string descriptionText;

};

enum StructureIndex { 
    Factory,
    FirePowerPlant,
    NuclearPowerPlant,
    FireDepartment,
    WindPowerPlant,
    ResidentialArea,
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
        {2, 1},         //size
        3,              //deltaClock
        false,          //earthquakeResist 耐震:true、不耐震:false
        false,          //flamable         易燃:true、不易燃:false
        3,              //electricCost
        ""              //description      先空著，我到時候再想 我好帥
    };
    structures[FirePowerPlant] = {
        "火力發電廠",
        {1, 1, 3, 0},
        0,
        {2, 2},
        3,
        true,
        true,
        3,
        ""
    };
    structures[WindPowerPlant] = {
        "風力發電廠",
        {1, 2, 0, 0},
        0,
        {1, 1},
        3,
        false,
        true,
        1,
        ""
    };
    structures[NuclearPowerPlant] = {
        "核能發電廠",
        {5, 5, 0, 0},
        0,
        {2, 2},
        0,            //if(NuclearDisaster==true) 15
        true,
        true,
        5,
        ""
    };
    structures[FireDepartment] = {
        "消防局",
        {2, 1, 0, 0},
        0,
        {1, 1},
        0,
        true,
        false,
        0,            //unkown
        ""
    };
    structures[ResidentialArea] = {
        "住宅區",
        {2, 0, 0, 0},
        0,
        {1, 1},
        1,
        false,
        true,
        1,
        ""
    };
    structures[Park] = {
        "公園",
        {1, 0, 0, 0},
        0,
        {2, 1},
        -1,
        false,
        true,
        0,            //unkown
        ""
    };
    structures[Coal] = {
        "煤礦場",
        {1, 2, 0, 0},
        0,
        {2, 2},
        2,
        false,
        false,
        0,            //unkown
        ""
    };
    structures[Farm] = {
        "農地",
        {0, 0, 0, 0}, //unkown
        0,
        {1, 1},
        0,
        true,
        true,
        0,            //unkown
        ""
    };
    structures[Storage] = {
        "糧倉",
        {2, 0, 0, 0},
        0,
        {2, 1},
        0,
        false,
        true,
        0,            //unkown
        ""
    };
}
