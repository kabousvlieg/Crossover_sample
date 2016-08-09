//
// Created by kobus on 2016/08/07.
//

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <lm.h>
#include <iphlpapi.h>
#include <malloc.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <unordered_set>
#include "tcpIpClient.h"
#include "exceptions.h"

#define DEFAULT_PORT "10000"
#define SERVER_IP "192.168.1.3"

SOCKET ConnectSocket = INVALID_SOCKET;
bool tcpIpClient::singleton = false;
std::string tcpIpClient::mac = "";
std::string secret = "AnyOldSharedSecret";

tcpIpClient::tcpIpClient(void) {
    WSADATA wsaData;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    int iResult;

    if (singleton == false) {
        mac = readMac();
        std::cout << "mac = " << mac << std::endl;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        throw new Except("WSAStartup failed");
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        WSACleanup();
        throw new Except("getaddrinfo failed");
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            throw new Except("socket failed");
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        throw new Except("Unable to connect to server!");
    }
}

void tcpIpClient::update(int mem, int cpu, int processes) {
    static int cnt = 0;
    // Send an initial buffer

    auto auth = std::hash<std::string>{}(mac + std::to_string(cnt) + secret);

    std::string sendbuf = "mac: " + mac + "; " + \
        "cnt: " + std::to_string(cnt) + "; " + \
        "auth: " + std::to_string(auth) + "; " + \
        "email: kobus@embeddedfool.net; " + \
        "memory: " + std::to_string(mem) + "; " + \
        "cpu: " + std::to_string(cpu) + "; " + \
        "processes: " + std::to_string(processes);
    int iResult;
    iResult = send( ConnectSocket, sendbuf.c_str(), sendbuf.length(), 0 );
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        throw new Except("send failed");
    }
    printf("Bytes Sent: %d\n", iResult);
}

tcpIpClient::~tcpIpClient() {
    int iResult;

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //Can't throw exception in dtor...
    }
}

std::string tcpIpClient::readMac(void) {
    IP_ADAPTER_INFO *info = NULL;
    DWORD size = 0;

    GetAdaptersInfo(info, &size);
    info = (IP_ADAPTER_INFO *)malloc(size);
    GetAdaptersInfo(info, &size);

    std::stringstream mac;
    mac  << std::hex << std::setfill('0') << std::setw(2);
    for (unsigned int i = 0; i < info->AddressLength; i++)
        mac << (int) info->Address[i];
    free(info);
    return mac.str();
}
