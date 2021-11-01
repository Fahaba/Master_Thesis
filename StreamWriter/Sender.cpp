#include "Sender.h"


Sender::Sender()
{
    if (int code = Initialize("127.0.0.1", "1337"))
    {
        printf("Error: %u", code);
        exit(code);
    }
}

int Sender::Initialize(std::string host, std::string port)
{
    int err;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iResult != 0) {
        err = WSAGetLastError();
        printf("WSAStartup failed with error: %d\n", err);
        WSACleanup();
        return err;
    }

    // alloc memory and initialize addrinfo struct
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if (iResult != 0) {
        err = WSAGetLastError();
        printf("getaddrinfo failed with error: %d\n", err);
        WSACleanup();
        return err;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        err = WSAGetLastError();
        printf("socket failed with error: %ld\n", err);
        freeaddrinfo(result);
        WSACleanup();
        return err;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        err = WSAGetLastError();
        printf("bind failed with error: %d\n", err);
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return err;
    }

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        err = WSAGetLastError();
        printf("listen failed with error: %d\n", err);
        closesocket(ListenSocket);
        WSACleanup();
        return err;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        err = WSAGetLastError();
        printf("accept failed with error: %d\n", err);
        closesocket(ListenSocket);
        WSACleanup();
        return err;
    }

    // No longer need server socket
    closesocket(ListenSocket);
    freeaddrinfo(result);
    return 0;
}

int Sender::Send(std::string msg)
{
    int err;
    iSendResult = send(ClientSocket, msg.c_str(), msg.size(), 0);
    if (iSendResult == SOCKET_ERROR)
    {
        err = WSAGetLastError();
        printf(" send failed with error: %d", err);
        closesocket(ClientSocket);
        WSACleanup();
        return err;
    }
    printf("Bytes sent: %u msgSize='%I64d'\n", iSendResult, msg.size());
}

int Sender::Shutdown()
{
    // shutdown the connection since we're done
    int err;
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        err = WSAGetLastError();
        printf("shutdown failed with error: %d\n", err);
        closesocket(ClientSocket);
        WSACleanup();
        return err;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}

Sender::~Sender()
{
}