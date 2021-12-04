#include "producer2.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <sys/time.h>

std::mutex print_lock2;

void Producer::print_interval(void) {
	uint64_t current;
	timespec tv;

	if (this->cycles == 0) {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		this->prev_time = tv.tv_sec * 1000 + tv.tv_nsec / 1000000;
	    cycles++;
	} else {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * 1000 + tv.tv_nsec / 1000000;

		if (this->cycles > 0) {
			print_lock2.lock();
			std::cout << std::this_thread::get_id() << " - Time elapsed: " + std::to_string(current - this->prev_time) + "\n";
			print_lock2.unlock();
		}
		this->prev_time = current;
	}
}

Producer::Producer(PeriodicTaskStore* task_store, enum InputVariable variable, std::ifstream *istrm, SensorDataQueue* data_queue) {
    this->task_store = task_store;
    this->id = variable;
    this->cycles = 0;
    std::thread thread_obj(&Producer::run_task, this, variable, istrm, data_queue);
	thread_obj.detach();
}

void Producer::notify() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one(); 
}

uint32_t Producer::wait() {
    std::unique_lock<std::mutex> lock(mutex);
    PeriodicTaskData data;
    while (true) {
        cv.wait(lock);
        // Make sure the condition holds
        if (task_store->getPeriodicTaskTimeData(this->id, &data) == 0
            && data.elapsed_ticks >= data.period) {
            break;
        }
    }
    return data.elapsed_ticks;
}

void Producer::run_task(enum InputVariable variable, std::ifstream *istrm, SensorDataQueue* data_queue) {
    while (true) {
        uint32_t elapsed_ticks = this->wait();
        // print_interval();
        this->task_store->resetElapsedTicks(this->id);
        // data_queue->enqueue(SensorData{RPM, 12345}); // TODO: remove after testing
        // return; // TODO: remove after testing

        double sensor_val;
        if (!istrm->is_open()) {
            throw "ERROR: " + InputDataFiles[variable] + " is not open";
        } else {
            std::string line;

            // Skip period-1 lines
            for (uint32_t i = 0; i < elapsed_ticks-1; i++)
            {
                std::getline(*istrm, line);
            }

            // Get next data value to send to consumer
            if (!std::getline(*istrm, line)) {
                std::cout << "End of " << InputDataFiles[variable] << " reached." << std::endl;
                return;
            } else {
                std::istringstream iss(line);
                if (!(iss >> sensor_val)) {
                    std::cerr << "Could not get data value from " << InputDataFiles[variable] << std::endl;
                }
            }

            // Send it to the consumer
            data_queue->enqueue(SensorData{variable, sensor_val});
        }
    }
}
