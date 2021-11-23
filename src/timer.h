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

	std::cout << "Thread started" << std::endl;

	this->cycles = 0;
}

void TaskTimer::AdjustInterval(int new_period) {
	this->timer_spec.it_interval.tv_sec = new_period / ONE_MILLION;
	this->timer_spec.it_interval.tv_nsec = (new_period % ONE_MILLION) * ONE_THOUSAND;
	timer_settime(this->timer, 0, &this->timer_spec, NULL);
	this->period = new_period;
}

void TaskTimer::print_interval(void) {
	uint64_t current;
	timespec tv;

	if (this->cycles == 0) {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		this->prev_time = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	} else {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;

		if (this->cycles > 0) {
			fprintf(stderr, "Time elapsed: %d millisecons\n",
					current - this->prev_time);
		}
		this->prev_time = current;
	}

	cycles++;
}

int TaskTimer::start_periodic_timer(uint64_t offset, int period) {
	struct sigevent sigev;
	const int signal = SIGALRM;
	int res;

	/* set timer parameters */
	this->timer_spec.it_value.tv_sec = offset / ONE_MILLION;
	this->timer_spec.it_value.tv_nsec = (offset % ONE_MILLION) * ONE_THOUSAND;
	this->timer_spec.it_interval.tv_sec = period / ONE_MILLION;
	this->timer_spec.it_interval.tv_nsec = (period % ONE_MILLION) * ONE_THOUSAND;

	sigemptyset(&this->sigst); // initialize a signal set
	sigaddset(&this->sigst, signal); // add SIGALRM to the signal set
	sigprocmask(SIG_BLOCK, &this->sigst, NULL); //block the signal

	/* set the signal event a timer expiration */
	memset(&sigev, 0, sizeof(struct sigevent));
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = signal;

	/* create timer */
	res = timer_create(CLOCK_MONOTONIC, &sigev, &this->timer);

	if (res < 0) {
		perror("Timer Create");
		exit(-1);
	}

	/* activate the timer */
	return timer_settime(this->timer, 0, &this->timer_spec, NULL);
}

template <typename... Args>
void TaskTimer::wait_next_activation(void (*callback) (int, Args...), Args... args) {
	int dummy;
	/* suspend calling process until a signal is pending */
	while (1) {
		sigwait(&this->sigst, &dummy);
		print_interval();
		callback(this->period, args...); //executes the task
	}
}

#endif // TASK_TIMER_H
