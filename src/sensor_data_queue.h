#ifndef SENSOR_DATA_QUEUE_H
#define SENSOR_DATA_QUEUE_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "shared_data.h"

typedef struct SensorData {
    enum InputVariable var;
    double data;
} SensorData;

class SensorDataQueue {
    public:
        /*
        * Adds the data to the queue and notifies any threads waiting for data.
        * @param data The new data to add to the queue.
        */
        void enqueue(SensorData data);

        /*
        * Waits for sensor data to be in the queue and then returns it.
        * @return The next sensor data.
        */
        SensorData waitForData();

    private:
        std::queue<SensorData> data_queue;
        std::mutex mutex;
        std::condition_variable cv;
};

#endif // SENSOR_DATA_QUEUE_H
