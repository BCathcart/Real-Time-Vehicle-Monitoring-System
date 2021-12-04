#ifndef PRODUCER2_H
#define PRODUCER2_H

#include "shared_data.h"
#include "sensor_data_queue.h"
#include "periodic_task_store.h"

#include <mutex>
#include <condition_variable>

class Producer {
    public:
        Producer(PeriodicTaskStore* task_store, enum InputVariable type, std::ifstream *istrm, SensorDataQueue* data_queue);
        void notify();

    private:
        void run_task(enum InputVariable type, std::ifstream *istrm, SensorDataQueue* data_queue);
        uint32_t wait();
        void print_interval();

        uint32_t id;
        PeriodicTaskStore* task_store;
        std::mutex mutex;
        std::condition_variable cv;

        // Testing fields
        int cycles;
	    uint64_t prev_time;
};

#endif //PRODUCER2_H
