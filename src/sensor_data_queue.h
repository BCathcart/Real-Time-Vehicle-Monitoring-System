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
        void enqueue(SensorData data);
        int size();
        SensorData waitForData();

    private:
        std::queue<SensorData> data_queue;
        std::mutex mutex;
        std::condition_variable cv;
};

#endif // SENSOR_DATA_QUEUE_H
