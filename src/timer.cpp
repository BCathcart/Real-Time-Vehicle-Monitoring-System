#include "timer.h"

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
	    cycles++;
	} else {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;

		if (this->cycles > 0) {
			std::cout << "Time elapsed: " + std::to_string(current - this->prev_time) + "\n";
		}
		this->prev_time = current;
	}
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
