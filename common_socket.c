#define _POSIX_C_SOURCE 200112L
#include "common_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h> 


void socket_create(socket_t* self, struct addrinfo* ptr){
    self->fd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
}

int socket_connect(socket_t* self, struct addrinfo* ptr){
    socket_create(self, ptr);
    if (self->fd == -1){
        printf("ERROR asignando socket \n");
        return -1; 
    }

    int peer = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (peer == -1){
        printf("Error de conexión de socket: %s \n", strerror(errno));
        socket_close(self);
        return -1;
    }
    shutdown(self->fd, SHUT_RD);
    return peer;
}

bool socket_bind(socket_t* self, struct sockaddr* ai_addr, 
                                                    socklen_t ai_addrlen){
    int option = 1;
    if (setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &option,
                                                    sizeof(option)) == -1){
        fprintf(stderr,"Error setsock: %s\n",strerror(errno));
        socket_close(self);
        return false;
    }
    int err = bind(self->fd, ai_addr, ai_addrlen);
    if (err == -1){
        printf("Error bind: %s \n", strerror(errno));
        socket_close(self);
        return false;
    }
    return true;
}
 
bool socket_listen(socket_t* self, int cant){
    int err = listen(self->fd, cant);
    if (err == -1){
        printf("Error listen: %s \n", strerror(errno));
        return false;
    }
    return true;
}

void socket_accept(socket_t* self, socket_t* skt_client){
    skt_client->fd = accept(self->fd, NULL, NULL);
    if (skt_client->fd == -1){
        fprintf(stderr, "Error fd accept: %s \n", strerror(errno));
    }
}
   
ssize_t socket_send(socket_t* self,unsigned char msg[], size_t msg_size){
    if (msg_size == 0) return 0;
    int tot_bytes = 0;
    while (tot_bytes < msg_size){
        ssize_t bytes = send(self->fd, &msg[tot_bytes], msg_size - tot_bytes,
                                                         MSG_NOSIGNAL);
        if (bytes == -1) {
            fprintf(stderr, "Error mandando mensaje: %s \n", strerror(errno));
            return -1;
        }
        if (bytes == 0)
            return 0;
        else
            tot_bytes += bytes;
    }
    return tot_bytes;
}

int socket_receive(socket_t* self, unsigned char msg[], size_t msg_size){
    if (msg_size == 0) return 0;
    ssize_t tot_bytes = 0;
    int new_bytes = 0;

    while (new_bytes < msg_size){
        new_bytes = recv(self->fd, &msg[tot_bytes], msg_size - tot_bytes, 0);
        if (new_bytes == -1 ){
            fprintf(stderr, "Error del receive new bytes: %s \n",
                                                             strerror(errno));
            return -1;
        }else if (new_bytes == 0){
            return 0;
        }else if (new_bytes < 64){
            return new_bytes;
        }
        tot_bytes += new_bytes;
    }
    return tot_bytes;
}

void socket_close(socket_t* self){
    if (!self) return;
    if (shutdown(self->fd, SHUT_RDWR) == -1) //cierro ambos lados
        printf("Error: %s", strerror(errno));
    if (close(self->fd) == -1) 
        printf("Error: %s", strerror(errno));
}
#ifdef PRUEBAS
#endif 
