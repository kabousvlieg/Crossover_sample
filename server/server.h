//
// Created by kobus on 2016/08/07.
//

#pragma once
#include "clientInfo.h"

class server {
public:
    server();
    bool Listen(clientInfo &rci);
private:

    bool readClientInfo(char recvbuf[512], clientInfo &rci);
};
