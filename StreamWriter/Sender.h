#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class Sender
{
public:
    Sender();
    ~Sender();
    int Shutdown();
    int Send(std::string msg);
    
private:
    
    int Initialize(std::string host, std::string port);

    WSADATA wsa;
    int iResult;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo* result = nullptr;
    struct addrinfo hints;
    int iSendResult;
};
