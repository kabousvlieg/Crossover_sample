#include <iostream>
#include <boost/log/trivial.hpp>
#include "server.h"
#include "database.h"
#include "mailer.h"

int main() {
    BOOST_LOG_TRIVIAL(info) << "Server app startup";
    try {
        auto srv = server();
        auto db = database();
        while (1)
        {
            clientInfo ci;
            auto valid = srv.Listen(ci); //TODO spawn each connection in its own thread
            if (valid)
                db.add(ci);
            //TODO replace hardcoded with XML file
            if ((ci.mem < 50) ||
                (ci.cpu > 80) ||
                (ci.processes > 50))
            {
                printf("\nSending mail\n");
                mailer::sendAlert(ci);
            }
        }
    }
    catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Caught exception:";
        BOOST_LOG_TRIVIAL(fatal) << e.what() << std::endl;
    }
    return 0;
}