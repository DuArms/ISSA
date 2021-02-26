#pragma once

#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <stdlib.h>

#define SIZE_SOCKET (sizeof(struct sockaddr_in))
#define MSG_LEN 516

#define PORT_RECIVE_4_SERVER 51121
#define PORT_SEND_2_SERVER 51120

#define true 1
#define false 0

int32_t create_socket(uint32_t port,in_addr_t address ,int (*what_2_do)(int32_t , const struct sockaddr *,socklen_t))
{
    int32_t socket_desc;
    struct sockaddr_in server;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        exit(1);
    }

    //Prepare the sockaddr_in structure
    server.sin_addr.s_addr = address;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);



    if (what_2_do(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror(" Error socket function failed");
        exit(1);
    }
    return socket_desc;
}

uint32_t accept_socket(uint32_t socket_desc, struct sockaddr_in client)
{

    uint32_t CONSTANT_SIZE = SIZE_SOCKET;
    uint32_t client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&CONSTANT_SIZE);

    if (client_sock < 0)
    {
        perror("accept failed");
        exit(1);
    }
    return client_sock;
}

int read_msg(uint32_t client_sock, uint8_t *client_message)
{
    int read_size = recv(client_sock, client_message, MSG_LEN, 0);
    if (read_size == -1)
    {
        perror("recv failed");
        exit(1);
    }
    return read_size;
}

void send_msg(uint32_t client_sock, uint8_t *client_message)
{
    if (send(client_sock, client_message, MSG_LEN, 0) < 0)
    {
        puts("Send failed");
        exit(1);
    }
}
