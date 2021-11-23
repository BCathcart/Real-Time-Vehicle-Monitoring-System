
#include "consumer.h"

#include <iostream>
#include <string>

void print_data(SensorData data) {
    // TODO: output data to the terminal

    // TODO: remove the following test code
    std::string msg = std::to_string(data.var) + ": " + std::to_string(data.data);
    std::cout << msg << std::endl;
}

void consumer(SensorDataQueue * data_queue) {
    while (true) {
        SensorData data = data_queue->waitForData();
        print_data(data);
    }
}