
#define _WIN32_WINNT
#define _POSIX_C_SOURCE 200112L
#include "client.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "common_socket.h"
#include <errno.h>
#include <netdb.h>


void client_init(client_t* client, const char* host, const char* port){
    client->host = host;
    client->port = port;
}

int client_connect(client_t* client){
    struct addrinfo hints;
    struct addrinfo* result;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    if ( getaddrinfo(client->host,client->port, &hints, &result) != 0 ){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        freeaddrinfo(result);
        return -1;
    }

    if (client_iterate_results(client, result) == -1){
        printf("Error en resultados");
        freeaddrinfo(result);
        return -1;
    }
    freeaddrinfo(result);
    return 0;
}

int client_iterate_results(client_t* client,addrinfo_t* result){
    struct addrinfo* ptr;   
    bool is_connected = false;

    for (ptr = result; ptr != NULL && is_connected == false;
                                             ptr = ptr->ai_next){
        int peer = socket_connect(client->skt, ptr);
        if (peer == -1) {
            continue;  
        }
        is_connected = (peer != -1); 
   }
   return is_connected;
}

int client_send(client_t* client, unsigned char* msg, size_t msg_size){
    return socket_send(client->skt, msg, msg_size);
}  

void client_close(client_t* client){
    if (!client) return;
    socket_close(client->skt);
}

#ifdef PRUEBAS
#endif
