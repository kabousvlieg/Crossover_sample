//
// Created by kobus on 2016/08/07.
//

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <stdlib.h>
#include "server.h"
#include "exceptions.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "10000"

SOCKET ListenSocket = INVALID_SOCKET;
WSADATA wsaData;
int rc;
std::string secret = "AnyOldSharedSecret";

server::server() {
    ListenSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    rc = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (rc != 0) {
        throw new Except("WSAStartup failed");
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    rc = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( rc != 0 ) {
        WSACleanup();
        throw new Except("getaddrinfo failed");
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw new Except("socket failed");
    }

    // Setup the TCP listening socket
    rc = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (rc == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        throw new Except("bind failed");
    }

    freeaddrinfo(result);

    rc = listen(ListenSocket, SOMAXCONN);
    if (rc == SOCKET_ERROR) {
        closesocket(ListenSocket);
        WSACleanup();
        throw new Except("listen failed");
    }
}

bool server::Listen(clientInfo &rci) {
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    SOCKET ClientSocket;

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        closesocket(ListenSocket);
        WSACleanup();
        throw new Except("accept failed");
    }

    std::cout << "Connected" << std::endl;
    rc = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (rc > 0) {
        printf("Bytes received: %d\n", rc);
        printf("->%s\n", recvbuf);
    }
    else if (rc == 0)
        printf("Connection closing...\n");
    else  {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        throw new Except("recv failed");
    }

    // shutdown the connection since we're done
    rc = shutdown(ClientSocket, SD_SEND);
    if (rc == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        throw new Except("shutdown failed");
    }

    return server::readClientInfo(recvbuf, rci);
}

bool server::readClientInfo(char *recvbuf, clientInfo &rci) {
    char const *delim = ";";
    int i = 0;
    char* strings[5];

    strings[i] = strtok(recvbuf, delim);
    while( strings[i] != NULL  )
    {
        strings[++i] = strtok(NULL, delim);
    }
    clientInfo ci = {
            strings[0] + 5,         //id
            strings[3] + 8,         //email
            atoi(strings[4] + 9),   //memory
            atoi(strings[5] + 6),   //cpu
            atoi(strings[6] + 12),  //processes
    };

    auto auth = std::hash<std::string>{}(ci.id + std::string(strings[1] + 6) + secret);
    if (std::string(strings[2] + 7) != std::to_string(auth))
        return false;
    rci = ci;
    return true;
}
