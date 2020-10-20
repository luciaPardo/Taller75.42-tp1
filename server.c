
#define _POSIX_C_SOURCE 200112L
#include "server.h" 
#include "common_socket.h" 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h> 
#include <sys/types.h>

#define MAX_CLIENTS 1




void server_init(server_t* server, const char* port){
    server->port = port;
}


int server_connect(server_t* self){
    struct addrinfo hints;
    struct addrinfo* results;
    struct addrinfo* ptr;
    bool is_connected = false;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL,self->port, &hints, &results) != 0){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        return -1;
    }
    for (ptr = results; ptr != NULL && is_connected == false;
                                                 ptr = ptr->ai_next){
        socket_create(self->skt,ptr);
        if (!server_listen(self, results)){
            fprintf(stderr,"Error levantando servidor.\n");
            freeaddrinfo(results);
            return -1;
        }
        is_connected = true;
    }
    freeaddrinfo(results);
    return 0;
}


bool server_listen(server_t* self, struct addrinfo* results){
    if (!socket_bind(self->skt, results->ai_addr, results->ai_addrlen))
        return false;
    if (!socket_listen(self->skt, MAX_CLIENTS))
        return false;
    return true;    
}

int server_accept_client(server_t* self){
    socket_accept(self->skt, self->client_skt);
    return 0;    
}

int server_recv(server_t* self, unsigned char* msg, size_t msg_size){
    return socket_receive(self->client_skt, msg, msg_size);
}

void server_close(server_t* self){
    socket_close(self->skt);
    socket_close(self->client_skt);
}
#ifdef PRUEBAS
#endif

