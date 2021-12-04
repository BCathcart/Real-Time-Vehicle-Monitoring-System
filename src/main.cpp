#include <iostream>
#include <thread>
#include <chrono>

#include "shared_data.h"
#include "consumer.h"
#include "frequency_adjuster.h"
#include "sensor_data_queue.h"
#include "periodic_task_store.h"
#include "timer2.h"
#include "producer2.h"

#define DEFAULT_TASK_PERIOD_SEC 5

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

    // Create a PeriodicTaskStore
    PeriodicTaskStore* periodic_task_store = new PeriodicTaskStore();
    periodic_task_store->addTask(FUEL_CONSUMPTION, new Producer(periodic_task_store, FUEL_CONSUMPTION, &file_streams[FUEL_CONSUMPTION], data_queue), DEFAULT_TASK_PERIOD_SEC);
    periodic_task_store->addTask(RPM, new Producer(periodic_task_store, RPM, &file_streams[RPM], data_queue), DEFAULT_TASK_PERIOD_SEC);
    periodic_task_store->addTask(COOLANT_TEMP, new Producer(periodic_task_store, COOLANT_TEMP, &file_streams[COOLANT_TEMP], data_queue), DEFAULT_TASK_PERIOD_SEC);
    periodic_task_store->addTask(GEAR, new Producer(periodic_task_store, GEAR, &file_streams[GEAR], data_queue), DEFAULT_TASK_PERIOD_SEC);
    periodic_task_store->addTask(SPEED, new Producer(periodic_task_store, SPEED, &file_streams[SPEED], data_queue), DEFAULT_TASK_PERIOD_SEC);

    // Create consumer
    std::thread consumer_thread(consumer, data_queue);
	consumer_thread.detach();

    // Create sporadic task
    std::thread frequency_adjuster_thread(listen_for_user_commands, periodic_task_store);
	frequency_adjuster_thread.detach();

    if (run_periodic_tasks(periodic_task_store) == -1) {
        std::cerr << "Could not run periodic tasts" << std::endl;
    }

    // TODO: destroy heap objects - doesn't really matter since program is exiting anyways

    return 0;
}
