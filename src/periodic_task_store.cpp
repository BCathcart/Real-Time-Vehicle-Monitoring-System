#include "periodic_task_store.h"
#include "producer2.h"

#include <iostream>

void PeriodicTaskStore::addTask(uint32_t id, Producer* producer, uint32_t period) {
    task_map[id] = new PeriodicTask{producer, PeriodicTaskData{period, 0}};
}

int PeriodicTaskStore::getPeriodicTaskTimeData(uint32_t id, PeriodicTaskData* data) {
    std::unique_lock<std::mutex> lock(mutex);
    if (task_map.count(id) == 0) {
        std::cerr << "Error: Task with ID=" + std::to_string(id) + " does not exist\n";
        return -1;
    }
    *data = task_map[id]->data;
    return 0;
}

void PeriodicTaskStore::updatePeriod(uint32_t id, uint32_t period) {
    std::unique_lock<std::mutex> lock(mutex);
    if (task_map.count(id) == 0) {
        std::cerr << "Error: Task with ID=" + std::to_string(id) + " does not exist\n";
        return;
    }
    PeriodicTask* task = task_map[id];
    task->data.period = period;
}

void PeriodicTaskStore::incrementElapsedTime() {
    std::unique_lock<std::mutex> lock(mutex);
    for (const auto &[id, task] : task_map) {
        task->data.elapsed_ticks++;
        // Notify the periodic task if it's time to run
        if (task->data.elapsed_ticks >= task->data.period) {
            task->producer->notify();
        }
    }
}

void PeriodicTaskStore::resetElapsedTicks(uint32_t id) {
    std::unique_lock<std::mutex> lock(mutex);
    if (task_map.count(id) == 0) {
        std::cerr << "Error: Task with ID=" + std::to_string(id) + " does not exist\n";
        return;
    }
    task_map[id]->data.elapsed_ticks = 0;
}
