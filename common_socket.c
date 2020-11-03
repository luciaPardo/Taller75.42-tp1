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

#define MAX_BYTES 64
#define MAX_CLIENTS 1

void socket_init(socket_t* self){
    self->fd = -1;
}

int client_iterate_results(socket_t* self, addrinfo_t* result){
    struct addrinfo* ptr;   
    bool is_connected = false;

    for (ptr = result; ptr != NULL && is_connected == false;
                                             ptr = ptr->ai_next){
        int peer = socket_connect(self, ptr);
        if (peer == -1) {
            continue;  
        }
        is_connected = (peer != -1); 
   }
   return is_connected;
}

int socket_client_connect(socket_t* self, const char* host, const char* port){
    int err = 0;
    struct addrinfo* result;
    if (socket_configurar_inf(self, false, port, host, &result) < 0){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        err = -1;
    }
    if (client_iterate_results(self, result) == -1){
        printf("Error en resultados");
        err = -1;
    }
    freeaddrinfo(result);
    return err;
}


int socket_configurar_inf(socket_t* self, bool serv_flag,const char* port,
                                                const char* host,
                                                struct addrinfo** results){
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = serv_flag ? AI_PASSIVE : 0;
    if (getaddrinfo(host, port, &hints, results) != 0){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        return -1;
    }
    return 0;
}


int socket_server_connect(socket_t* self, const char* port){
    struct addrinfo* results;
    struct addrinfo* ptr;
    bool is_connected = false;
    if (socket_configurar_inf(self, true, port, NULL, &results) < 0){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        return -1;
    }
    for (ptr = results; ptr != NULL && is_connected == false;
                                                 ptr = ptr->ai_next){
        socket_create(self, ptr);
        if (!socket_bind(self, results->ai_addr, results->ai_addrlen))
            return -1;
        if (!socket_listen(self, MAX_CLIENTS))
            return -1;
        is_connected = true;
    }
    freeaddrinfo(results);
    return 0;
}


void socket_create(socket_t* self, struct addrinfo* ptr){
    self->fd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
}

int socket_connect(socket_t* self, struct addrinfo* ptr){
    socket_create(self, ptr);
    if (self->fd == -1){
        printf("ERROR asignando socket \n");
        socket_close(self);
        return -1; 
    }

    int peer = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (peer == -1){
        printf("Error de conexión de socket: %s \n", strerror(errno));
        socket_close(self);
        return -1;
    }
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
        if (new_bytes == -1){
            fprintf(stderr, "Error del receive new bytes: %s \n",
                                                             strerror(errno));
            return -1;
        }else if (new_bytes == 0){
            return tot_bytes;
        }else if (new_bytes < MAX_BYTES){
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
