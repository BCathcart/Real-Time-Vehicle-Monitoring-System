#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <string>
#include <queue>          // std::queue
#include <fstream>

#define NUM_INPUT_VARS 5

#define INPUT_DATA_PATH "data/"

enum InputVariable
{
    FUEL_CONSUMPTION,
    RPM,
    COOLANT_TEMP,
    GEAR,
    SPEED
};

//extern int periods[] = {};

extern std::string InputDataFiles[];
// extern std::ifstream inputDataFileStreams[NUM_INPUT_VARS];

// Put the data queue (my custom database class) here

#endif //SHARED_DATA_H
