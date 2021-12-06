
#include "consumer.h"

#include <iostream>
#include <string>


bool f = false;
bool r = false;
bool c = false;
bool g = false;
bool s = false;

void print_data(SensorData data) {
    // TODO: remove the following test code
    // std::string msg = std::to_string(data.var) + ": " + std::to_string(data.data);
    // std::cout << msg << std::endl;


	switch(std::stoi(std::to_string(data.var))) {
	case 0:
		f = true;

		std::cout << std::right
				<< "| " << std::setw(17) << std::stoi(std::to_string(data.data))
				<< "| " << std::setw(6)  << ""
				<< "| " << std::setw(20) << ""
				<< "| " << std::setw(5)  << ""
				<< "| " << std::setw(6)  << "" << "|" << std::endl;

		break;
	case 1:
		r = true;
		std::cout << std::right
				<< "| " << std::setw(17) << ""
				<< "| " << std::setw(6)  << std::stoi(std::to_string(data.data))
				<< "| " << std::setw(20) << ""
				<< "| " << std::setw(5)  << ""
				<< "| " << std::setw(6)  << "" << "|" << std::endl;

		break;
	case 2:
		c = true;
		std::cout << std::right
				<< "| " << std::setw(17) << ""
				<< "| " << std::setw(6)  << ""
				<< "| " << std::setw(20) << std::stoi(std::to_string(data.data))
				<< "| " << std::setw(5)  << ""
				<< "| " << std::setw(6)  << "" << "|" << std::endl;

		break;
	case 3:
		g = true;
		std::cout << std::right
				<< "| " << std::setw(17) << ""
				<< "| " << std::setw(6)  << ""
				<< "| " << std::setw(20) << ""
				<< "| " << std::setw(5)  << std::stoi(std::to_string(data.data))
				<< "| " << std::setw(6)  << "" << "|" << std::endl;

		break;
	case 4:
		s = true;
		std::cout << std::right
				<< "| " << std::setw(17) << ""
				<< "| " << std::setw(6)  << ""
				<< "| " << std::setw(20) << ""
				<< "| " << std::setw(5)  << ""
				<< "| " << std::setw(6)  << std::stoi(std::to_string(data.data)) << "|" << std::endl;

		break;
	default:
		std::cout << "The InputVariable is undefined" << std::endl;
	}

	if (f == true && r == true && c == true && g == true && s == true){
		std::cout << "|------------------+-------+---------------------+------+-------| <- Longest period has finished\n";
		f = false;
		r = false;
		c = false;
		g = false;
		s = false;
	}
}

void consumer(SensorDataQueue * data_queue) {
	std::cout << ".------------------.-------.---------------------.------.-------.\n"
			<< std::left
			<< std::setw(20) << "| Fuel Consumption |"
			<< std::setw(8) << "  RPM  |"
			<< std::setw(22) << " Coolant Temperature |"
			<< std::setw(7) << " Gear |"
			<< std::setw(8) << " Speed |\n"
			<< "|------------------+-------+---------------------+------+-------|\n";

    while (true) {
        SensorData data = data_queue->waitForData();
        print_data(data);
    }
}
