#include <iostream>
#include <thread>
#include <chrono>

#include "shared_data.h"
#include "timer.h"
#include "producer.h"
#include "consumer.h"
#include "frequency_adjuster.h"
#include "sensor_data_queue.h"

#define DEFAULT_TASK_PERIOD_US 5000000

std::string InputDataFiles[] = {"fuel_consumption.txt", "engine_speed.txt", "engine_coolant_temp.txt", "current_gear.txt", "vehicle_speed.txt"};


void open_sensor_data_file_streams(std::ifstream * input_files) {
    for (int i = 0; i < NUM_INPUT_VARS; i++) {
		input_files[i] = std::ifstream(INPUT_DATA_PATH + InputDataFiles[i]);
        if (!input_files[i].is_open()) {
            throw "Could not open " + InputDataFiles[i];
        }
    }
}

int main() {
    // Open input files
    std::ifstream file_streams[NUM_INPUT_VARS];
    try {
        open_sensor_data_file_streams(file_streams);
    } catch(std::string err) {
        std::cerr << "Could not open input files: " << err << std::endl;
    }

    // Create a SensorDataQueue
    SensorDataQueue* data_queue = new SensorDataQueue();

    // Create producers
    // TaskTimer* producer_timers[FUEL_CONSUMPTION];
    TaskTimer** producer_timers = new TaskTimer*[NUM_INPUT_VARS];
    producer_timers[FUEL_CONSUMPTION] = new TaskTimer(DEFAULT_TASK_PERIOD_US, producer, FUEL_CONSUMPTION, &file_streams[FUEL_CONSUMPTION], data_queue);
    // producer_timers[RPM] = new TaskTimer(DEFAULT_TASK_PERIOD_US, producer, RPM, &file_streams[RPM], data_queue);
    // producer_timers[COOLANT_TEMP] = new TaskTimer(DEFAULT_TASK_PERIOD_US, producer, COOLANT_TEMP, &file_streams[COOLANT_TEMP], data_queue);
    // producer_timers[GEAR] = new TaskTimer(DEFAULT_TASK_PERIOD_US, producer, GEAR, &file_streams[GEAR], data_queue);
    // producer_timers[SPEED] = new TaskTimer(DEFAULT_TASK_PERIOD_US, producer, SPEED, &file_streams[SPEED], data_queue);

    // Time adjustment testing - works!
    // std::this_thread::sleep_for(std::chrono::milliseconds(11000));
    // producer_timers[0]->AdjustInterval(1000000);

    // Create consumer
    std::thread consumer_thread(consumer, data_queue);
	consumer_thread.detach();

    // Create sporadic task
    std::thread frequency_adjuster_thread(listen_for_user_commands, producer_timers);
	frequency_adjuster_thread.detach();

    while (1) {}

    // TODO: destroy heap objects - doesn't really matter since program is exiting anyways

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <limits.h>

// #include <fstream>
// #include <iostream>


// int main( void )
// {
//     char* cwd;
//     char buff[PATH_MAX + 1];

//     cwd = getcwd( buff, PATH_MAX + 1 );
//     if( cwd != NULL ) {
//         printf( "My working directory is %s.\n", cwd );
//     }

//     std::ifstream istrm;
//     istrm.open("test.txt");
//     std::cout << istrm.is_open();


//     return EXIT_SUCCESS;
// }

