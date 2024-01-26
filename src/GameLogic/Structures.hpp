#include <string>

struct Feature
{
    bool shockResist;
    bool flamable;
    unsigned int electricCost;
    std::string getDescriptionText();
};

struct Resources
{
    int log, iron, coal, food;
};

struct Size { unsigned int width, height; };

class Structure
{
private:

    std::string name;

    Resources resourceCost;

    int populationGain;

    Size size;

    int deltaClock;

    Feature feature;

public:

};