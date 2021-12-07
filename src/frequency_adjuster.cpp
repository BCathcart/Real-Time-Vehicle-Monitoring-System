#include "frequency_adjuster.h"
#include <string>
#include <vector>
#include <iostream>

void listen_for_user_commands(PeriodicTaskStore* periodic_task_store) {
    std::string taskChange;
    std::string delimiter = ",";
    std::vector<int> newPeriod;
	while (1){
		std::cin >> taskChange; //must in the form 'task#','newPeriod'

        try {
            size_t pos = 0;
            std::string token;
            while ((pos = taskChange.find(delimiter)) != std::string::npos) {
                token = taskChange.substr(0, pos);
                newPeriod.push_back(std::stoi(token));
                taskChange.erase(0, pos + delimiter.length());
            }
            newPeriod.push_back(std::stoi(taskChange));

            if (newPeriod[1] < 1) {
                std::cout << newPeriod[1] << std::endl;
                std::cerr << "Error: The new period must >= 1." << std::endl;
                newPeriod.clear();
                continue;
            }

            std::cout << "Task: " << newPeriod[0] << " New Period: " << newPeriod[1] << std::endl;
            periodic_task_store->updatePeriod(newPeriod[0], newPeriod[1]);
            newPeriod.clear();
        } catch (...) {
            std::cerr << "Error: Input must be in the form '<uint>,<uint>' which represents 'variable #','newPeriod'" << std::endl;
            newPeriod.clear();
        }
	}
}
