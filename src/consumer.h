#include "sensor_data_queue.h"

/*
 * Infinitely loops, consuming the data produced by all producers
 * and outputing it to the terminal.
 * @param data_queue The SensorDataQueue object containing data from the producers.
 */
void consumer(SensorDataQueue * data_queue);