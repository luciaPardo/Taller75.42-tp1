#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common_socket.h" 

struct server{
    const char* port;
    socket_t* skt;
    socket_t* client_skt;
};

typedef struct server server_t;

void server_init(server_t* server, const char* port);

int server_connect (server_t* self);

bool server_listen(server_t* self, struct addrinfo* results);

int server_send(server_t* self, unsigned char* msg, size_t msg_size);

int server_recv(server_t* self, unsigned char* msg, size_t msg_size);

int server_accept_client(server_t* self);

void server_close(server_t* self);

#endif