#include "server.h"
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

void launch(struct server *server)
{
    printf("=============== Waiting For Connection ================\n");

    struct sockaddr_in client;
    int client_size = sizeof(client);
    SOCKET new_socket = accept(server->socket, (struct sockaddr *)&client, &client_size);

    if (new_socket == INVALID_SOCKET)
    {
        printf("Failed to accept connection: %d\n", WSAGetLastError());
        return;
    }

    char buffer[500000] = {0};
    recv(new_socket, buffer, sizeof(buffer), 0);
    printf("Received request:\n%s\n", buffer);

    char *hello = "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/plain\r\n"
                  "Content-Length: 39\r\n"
                  "\r\n"
                  "Bravo-6 to gold eagle actual. GOING DARK";
    send(new_socket, hello, strlen(hello), 0);

    closesocket(new_socket);
}
#include "server.h"
#include <stdio.h>
#include <winsock2.h>

// Function prototype
void launch(struct server *server);

int main()
{
    struct server my_server = serverconstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 10, launch);
    my_server.launch(&my_server);

    WSACleanup();

    return 0;
}
