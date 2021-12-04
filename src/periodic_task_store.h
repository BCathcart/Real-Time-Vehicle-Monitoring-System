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
        /*
        * Adds a new PeriodicTask to the store.
        * @param id The unique ID of the task.
        * @param producer The Producer object associated with the producer task.
        *   NOTE: if different types of periodic tasks are added in the future,
        *   this can be a generic PeriodicTask type.
        * @param period The period in seconds of the task.
        */
        void addTask(uint32_t id, Producer* producer, uint32_t period);

        /*
        * Returns the task's data
        * @param id The ID of the task.
        * @param[out] data The task's data.
        * @return -1 if the task doesn't exist, 0 if it does. 
        */
        int getPeriodicTaskTimeData(uint32_t id, PeriodicTaskData* data);

        /*
        * Updates the task's period.
        * @param id The ID of the task.
        * @param period The new period of the task.
        */
        void updatePeriod(uint32_t id, uint32_t period);

        /*
        * Increments the elapsed ticks for all periodic tasks and notifies the task
        * to run if elapsed_ticks > period. Each tick corresponds to 1 second.
        */
        void incrementElapsedTime();

        /*
        * Resets the elapsed ticks for the task.
        * @param id The task ID.
        */
        void resetElapsedTicks(uint32_t id);

    private:
        std::mutex mutex;
        std::unordered_map<uint32_t, PeriodicTask*> task_map;
};

#endif //PERIODIC_TASK_STORE_H