#ifndef TASK_TIMER_H
#define TASK_TIMER_H

#include "shared_data.h"

#include <thread>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#define ONE_THOUSAND	1000
#define ONE_MILLION		1000000
/* offset and period are in microsends. */
#define OFFSET 1000000

class TaskTimer
{
public:
    template <typename... Args> TaskTimer(int period, void (*callback)(int, Args... args), Args... args);
    void AdjustInterval(int new_period);

private:
    timer_t timer;
    struct itimerspec timer_spec;
    sigset_t sigst;
    std::thread *thread_obj; // TODO: destroy in destructor(?)
    int period;

	int cycles;
	uint64_t prev_time;

    int start_periodic_timer(uint64_t offset, int period);
    template <typename... Args> void wait_next_activation(void (*callback)(int, Args... args), Args... args);
	void print_interval();
};

/* Construct a new timer */
template<typename... Args>
TaskTimer::TaskTimer(int period, void (*callback) (int, Args... args), Args... args) {
	this->period = period;
	//set and activate a timer
	if (start_periodic_timer(1000, period) < 0){
		throw ("Start periodic timer");
	}

	std::thread thread_obj(&TaskTimer::wait_next_activation<Args...>, this, callback, args...);
	thread_obj.detach();

	std::cout << "Timer thread started" << std::endl;
	std::cout << &this->sigst << std::endl;

	this->cycles = 0;
}

template <typename... Args>
void TaskTimer::wait_next_activation(void (*callback) (int, Args...), Args... args) {
	int dummy;
	/* suspend calling process until a signal is pending */
	while (1) {
		sigwait(&this->sigst, &dummy);
		print_interval();
		// producer(FUEL_CONSUMPTION, &file_streams[FUEL_CONSUMPTION], data_queue)
		callback(this->period, args...); //executes the task
	}
}


#endif // TASK_TIMER_H
