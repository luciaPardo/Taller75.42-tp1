#ifndef SOCKET_H
#define SOCKET_H


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>


struct socket{
    int fd;
};
  
typedef struct socket socket_t;

typedef struct addrinfo addrinfo_t;

/*Inicializa un socket con fd -1*/
void socket_init(socket_t* self);

/*crea un socket*/
void socket_create(socket_t* self, struct addrinfo* ptr);

/*Conecta socket*/
int socket_connect(socket_t* self, struct addrinfo* ptr);

/*Convierte identificadores en direcciones*/
bool socket_bind(socket_t* self, struct sockaddr* ai_addr, 
                                                socklen_t ai_addrlen);
/*Escucha por conexiones*/
bool socket_listen(socket_t* self, int cant);

/*Acepta un cliente y setea su file descriptor*/
void socket_accept(socket_t* self, socket_t* skt_client);

/*Manda mensajes y devuelve los bytes enviados*/
ssize_t socket_send(socket_t* self, unsigned char buf[], size_t len);

/*Recibe los mensajes y devuelve los bytes recibidos*/
int socket_receive(socket_t* self, unsigned char buf[], size_t length);

/*Cierra un socket*/
void socket_close(socket_t* self);

#endif 