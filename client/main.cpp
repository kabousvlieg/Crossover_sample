#include <iostream>
#include <boost/log/trivial.hpp>
#include <synchapi.h>
#include "monitor.h"
#include "tcpIpClient.h"

//const int monitorInterval = 1000*60*5; //5 minutes in milliseconds
const int monitorInterval = 1000 * 15; //15 seconds in milliseconds

int main() {
	BOOST_LOG_TRIVIAL(info) << "Client app startup";
	try {
		while (1)
		{
			auto mem = monitor::memory();
			auto cpu = 0;
			auto processes = 0;
			std::cout << "Memory available = " << mem << std::endl;
			auto client = tcpIpClient();
			client.update(mem, cpu, processes);
			Sleep(monitorInterval);
		}
	}
	catch (std::exception& e) {
		BOOST_LOG_TRIVIAL(fatal) << "Caught exception:";
		BOOST_LOG_TRIVIAL(fatal) << e.what() << std::endl;
	}
	return 0;
}
