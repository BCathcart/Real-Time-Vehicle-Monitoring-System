
#include "consumer.h"

#include <iostream>
#include <string>
#include <sys/time.h>

std::string get_interval(void) {
    static int cycles;
    static uint64_t prev_time;
	uint64_t current;
	timespec tv;

	if (cycles == 0) {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		prev_time = tv.tv_sec * 1000 + tv.tv_nsec / 1000000;
	    cycles++;
	} else {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * 1000 + tv.tv_nsec / 1000000;
		std::string ret = " - Time elapsed: " + std::to_string((current - prev_time + 500)/1000) + "s";
		prev_time = current;
        return ret;
	}
    return "";
}

void print_data(SensorData data) {
    static bool f = false;
    static bool r = false;
    static bool c = false;
    static bool g = false;
    static bool s = false;

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
		std::cout << "|------------------+-------+---------------------+------+-------| <- Longest period has finished" + get_interval() << std::endl;
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
