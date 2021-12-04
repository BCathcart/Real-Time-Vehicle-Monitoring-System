#ifndef PERIODIC_TASK_STORE_H
#define PERIODIC_TASK_STORE_H

// TODO: protect this store with a mutex
#include <mutex>
#include <unordered_map>

class Producer;

typedef struct PeriodicTaskData {
    uint32_t period;
    uint32_t elapsed_ticks;
} PeriodicTaskData;

typedef struct PeriodicTask {
    Producer* producer;
    PeriodicTaskData data;
} PeriodicTask;

class PeriodicTaskStore {
    public:
        void addTask(uint32_t id, Producer* producer, uint32_t period);
        int getPeriodicTaskTimeData(uint32_t id, PeriodicTaskData* data);
        void updatePeriod(uint32_t id, uint32_t period);
        void incrementElapsedTime();
        void resetElapsedTicks(uint32_t id);

    private:
        std::mutex mutex;
        std::unordered_map<uint32_t, PeriodicTask*> task_map;
};

#endif //PERIODIC_TASK_STORE_H