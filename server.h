#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib") // Ensure linking Winsock

struct server
{
    int domain;
    int service;
    int protocol;
    u_long iface;
    int port;
    int backlog;

    struct sockaddr_in address;
    SOCKET socket; // Changed from int to SOCKET for Windows compatibility

    void (*launch)(struct server *server);
};

// Corrected function prototype
struct server serverconstructor(int domain, int service, int protocol, u_long iface, int port, int backlog, void (*launch)(struct server *server));

#endif
