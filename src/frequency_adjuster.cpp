#include "frequency_adjuster.h"

void listen_for_user_commands(TaskTimer* producer_timers[]) {
    // TODO: listen for commands and call AdjustInterval() on the timer accordingly
    int newPeriod;

	while (1){
		std::cin >> newPeriod;
		std::cout << "New Period: " << newPeriod << std::endl;

		producer_timers[0]->AdjustInterval(newPeriod * 1000000);
	}
    // input = scanf()

    // input == "x"
    // // turn off output - std::cout.setstate(std::ios_base::failbit);

    // producer_timers[RPM].AdjustInterval()
}