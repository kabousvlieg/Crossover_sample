#include <iostream>
#include <boost/log/trivial.hpp>
#include "tcpIpServer.h"

int main() {
    BOOST_LOG_TRIVIAL(info) << "Server app startup";
    try {
        auto server = new tcpIpServer();
        server->Listen();
        while (1)
        {
        }
    }
    catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Caught exception:";
        BOOST_LOG_TRIVIAL(fatal) << e.what() << std::endl;
    }
    return 0;
}