#include "shared_data.h"
#include "sensor_data_queue.h"

void producer(int period, enum InputVariable variable, std::ifstream *istrm, SensorDataQueue* data_queue);
