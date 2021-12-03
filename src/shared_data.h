#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <string>
#include <queue>          // std::queue
#include <fstream>

#define NUM_INPUT_VARS 5

#define INPUT_DATA_PATH "/data/home/project_data/"

enum InputVariable
{
    FUEL_CONSUMPTION,
    RPM,
    COOLANT_TEMP,
    GEAR,
    SPEED
};

extern std::string InputDataFiles[];

#endif //SHARED_DATA_H
