#include "frequency_adjuster.h"

#include <iostream>

void listen_for_user_commands(PeriodicTaskStore* periodic_task_store) {
    // TODO: listen for commands and call AdjustInterval() on the timer accordingly
    int newPeriod;
	while (1){
		std::cin >> newPeriod;
		std::cout << "New Period: " << newPeriod << std::endl;

		periodic_task_store->updatePeriod(0, newPeriod * 1000000);
	}
    // input = scanf()

    // input == "x"
    // // turn off output - std::cout.setstate(std::ios_base::failbit);

    // producer_timers[RPM].AdjustInterval()
}