#include <iostream>
#include <boost/log/trivial.hpp>
#include <unistd.h>
#include <synchapi.h>
#include "monitor.h"

const int monitorInterval = 1000*1000*60*5; //5 minutes in milliseconds

int main() {
    BOOST_LOG_TRIVIAL(info) << "Client app startup";
    while (1)
    {
        auto mem = monitor::memory();
        std::cout << "Memory available = " << mem << std::endl;
        Sleep(monitorInterval);
    }
    return 0;
}