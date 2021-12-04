#ifndef TIMER2_H
#define TIMER2_H

#include "periodic_task_store.h"

/*
* Infinitely loops, incrementing the number of ticks in the PeriodicTaskStore
* every time the timer is triggered (once per second).
* @param periodic_task_store The periodic task store.
* @return -1 if failure occurs, loops infinitely otherwise.
*/
int run_periodic_tasks(PeriodicTaskStore* periodic_task_store);

#endif //TIMER2_H
