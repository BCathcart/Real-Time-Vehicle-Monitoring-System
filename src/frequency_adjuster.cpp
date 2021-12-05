#include "frequency_adjuster.h"
#include <string>
#include <vector>
#include <iostream>

void listen_for_user_commands(PeriodicTaskStore* periodic_task_store) {
    // TODO: listen for commands and call AdjustInterval() on the timer accordingly
    std::string taskChange;
    std::string delimiter = ",";
    std::vector<int> newPeriod;
	while (1){
		std::cin >> taskChange; //must in the form 'task#','newPeriod'

		size_t pos = 0;
		std::string token;
		while ((pos = taskChange.find(delimiter)) != std::string::npos) {
		    token = taskChange.substr(0, pos);
		    newPeriod.push_back(std::stoi(token));
		    taskChange.erase(0, pos + delimiter.length());
		}
		newPeriod.push_back(std::stoi(taskChange));

		std::cout << "Task: " << newPeriod[0] << " New Period: " << newPeriod[1] << std::endl;
		periodic_task_store->updatePeriod(newPeriod[0], newPeriod[1]);
		newPeriod.clear();
	}
    // input = scanf()

    // input == "x"
    // // turn off output - std::cout.setstate(std::ios_base::failbit);

    // producer_timers[RPM].AdjustInterval()
}
