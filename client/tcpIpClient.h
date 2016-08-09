//
// Created by kobus on 2016/08/07.
//

#pragma once

#include <string>

class tcpIpClient {
public:
    tcpIpClient(void);
    void update(int, int i, int i1);
    ~tcpIpClient(void);
private:
    static bool singleton; //Used to create a unique identifier
    static std::string mac;
    std::string readMac(void);
};
