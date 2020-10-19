
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
    server->skt = (server->skt);
    server->client_skt = (server->client_skt);
}


int server_connect (server_t* self){
    struct addrinfo hints;
    struct addrinfo* results;
    struct addrinfo* ptr;
    bool is_connected = false;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL,self->port, &hints, &results) != 0 ){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        return -1;
    }
    for (ptr = results; ptr != NULL && is_connected == false;
                                                 ptr = ptr->ai_next){
        socket_create(self->skt,ptr);
        if(! server_listen(self, results)){
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
    if(!socket_listen(self->skt, MAX_CLIENTS))
        return false;
    return true;    
}

int server_accept_client(server_t* self){
    socket_accept(self->skt, self->client_skt);
    return 0;    
}

int server_send(server_t* self, unsigned char* msg, size_t msg_size){
    size_t bytes_sent = 0;
    size_t total_sent = 0;
    bool bad_socket = false;
    while (bytes_sent < msg_size && !bad_socket){
        bytes_sent = socket_send(self->skt, msg, msg_size);
        if (bytes_sent == 0) return 0;
        if (bytes_sent == -1) bad_socket = true;
        else{
            total_sent += bytes_sent;
        }
    }
    if (!bad_socket) return (int) total_sent;
    else{
        fprintf(stderr,"Falla enviando mensaje con error: %s\n",strerror(errno));
        return -1;
    }
}  

int server_recv(server_t* self, unsigned char* msg, size_t msg_size){
    return socket_receive (self->client_skt, msg, msg_size);
}

void server_close(server_t* self){
    socket_close(self->skt);
}
#ifdef PRUEBAS
#endif


