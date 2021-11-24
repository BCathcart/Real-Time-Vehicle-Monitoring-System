
#include "sensor_data_queue.h"

void SensorDataQueue::enqueue(SensorData data) {
    std::unique_lock<std::mutex> lock(mutex);
    data_queue.push(data);
    cv.notify_one(); // There should only ever be the one consumer waiting
}

SensorData SensorDataQueue::waitForData() {
    std::unique_lock<std::mutex> lock(mutex);
    while (data_queue.empty()) {
        cv.wait(lock);  // TODO: switch to wait_until()
    }
    SensorData data = data_queue.front();
    data_queue.pop();
    return data;
}
