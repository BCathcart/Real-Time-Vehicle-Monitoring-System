#ifndef PRODUCER2_H
#define PRODUCER2_H

#include "shared_data.h"
#include "sensor_data_queue.h"
#include "periodic_task_store.h"

#include <mutex>
#include <condition_variable>

class Producer {
    public:
        /*
        * Constructs a producer and launches a thread that runs the task.
        * @param task_store The periodic task store.
        * @param variable The sensor input variable type of the data being produced.
        * @param istrm Pointer to the sensor data file input stream.
        * @param data_queue The SensorDataQueue for sending sensor data to the consumer.
        */
        Producer(PeriodicTaskStore* task_store, enum InputVariable variable, std::ifstream *istrm, SensorDataQueue* data_queue);

        /*
        * Notifies the producer task that it is ready to run.
        */
        void notify();

    private:
        /*
        * Infinitely loops, running the producer task when it has been notified
        * (this->notify() is called) that the next sensor data value can be fetched.
        * @param variable The sensor input variable type of the data being produced.
        * @param istrm Pointer to the sensor data file input stream.
        * @param data_queue The SensorDataQueue for sending sensor data to the consumer.
        */
        void run_task(enum InputVariable variable, std::ifstream *istrm, SensorDataQueue* data_queue);

        /*
        * Waits until a period has elapsed and the next sensor data can be read.
        * @return The elapsed ticks since the task last ran.
        */
        uint32_t wait();

        /*
        * Prints time interval between task runs for debugging purposes.
        */
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
