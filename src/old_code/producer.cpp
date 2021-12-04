#include <fstream>
#include <iostream>
#include <sstream>

#include "producer.h"

// Can add a std::pair<DATA_TYPE, VALUE> to a queue that consumed by the consumer, then update a semaphore that the consumer is waiting on

// Timer will call this function and pass in the data_type so it knows what file to read from
void producer(int period, enum InputVariable variable, std::ifstream *istrm, SensorDataQueue* data_queue) {
	// std::cout << "producer()" << std::endl;

    data_queue->enqueue(SensorData{RPM, 12345}); // TODO: remove after testing

    return; // TODO: remove after testing

    double sensor_val;
    if (!istrm->is_open()) {
        throw "ERROR: " + InputDataFiles[variable] + " is not open";
    } else {
        // Read the next value and send it to the queue

        std::string line;

        // Skip period-1 lines
        for (int i = 0; i < period-1; i++)
        {
            std::getline(*istrm, line);
        }

        // Get next data value to send to consumer
        if (!std::getline(*istrm, line)) {
            std::cout << "End of " << InputDataFiles[variable] << " reached." << std::endl;
            return;
        } else {
            std::istringstream iss(line);
            double sensor_val;
            if (!(iss >> sensor_val)) {
                std::cerr << "Could not get data value from " << InputDataFiles[variable] << std::endl;
            }
        }

        // Send it to the consumer
            data_queue->enqueue(SensorData{variable, sensor_val});
    }
}
