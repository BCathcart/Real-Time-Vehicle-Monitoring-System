#ifndef FREQUENCY_ADJUSTER_H
#define FREQUENCY_ADJUSTER_H

#include "periodic_task_store.h"

/*
 * Listens for user commands that adjust the periods of the producers.
 * @param periodic_task_store The periodic task store where the producer periods are updated.
 */
void listen_for_user_commands(PeriodicTaskStore* periodic_task_store);

#endif // FREQUENCY_ADJUSTER_H
