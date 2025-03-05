#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Ensure Winsock library is linked

struct server serverconstructor(int domain, int service, int protocol, u_long iface, int port, int backlog, void (*launch)(struct server *server))
{
    struct server s;

    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        exit(1);
    }

    s.domain = domain;
    s.service = service;
    s.protocol = protocol;
    s.iface = iface;
    s.port = port;
    s.backlog = backlog;
    s.launch = launch;

    s.address.sin_family = domain;
    s.address.sin_port = htons(port);
    s.address.sin_addr.s_addr = htonl(iface);

    // Create socket
    s.socket = socket(domain, service, protocol);
    if (s.socket == INVALID_SOCKET)
    {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    // Bind socket
    if (bind(s.socket, (struct sockaddr *)&s.address, sizeof(s.address)) == SOCKET_ERROR)
    {
        printf("Failure in binding to the socket: %d\n", WSAGetLastError());
        closesocket(s.socket);
        WSACleanup();
        exit(1);
    }

    // Listen on socket
    if (listen(s.socket, s.backlog) == SOCKET_ERROR)
    {
        printf("Could not listen on the socket: %d\n", WSAGetLastError());
        closesocket(s.socket);
        WSACleanup();
        exit(1);
    }

    s.launch = launch;
    return s;
}
