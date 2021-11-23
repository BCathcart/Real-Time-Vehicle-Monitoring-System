#include "shared_data.h"
#include "sensor_data_queue.h"

void producer(int period, enum InputVariable type, std::ifstream *istrm, SensorDataQueue* data_queue);
