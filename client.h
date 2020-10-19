#ifndef CLIENT_H
#define CLIENT_H
#define _WIN32_WINNT
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <netdb.h>
#include "common_socket.h"

typedef struct client{
    const char* host;
    const char* port;
    socket_t* skt;
} client_t;


/*Inicializa el cliente */
void client_init(client_t* client, const char* host, const char* port);

/*conecta el cliente con un servidor */
int client_connect(client_t* client);

/*Itera los resultados de getaddrinfo buscando direcciones válidas */
int client_iterate_results(client_t* client,struct addrinfo* result);

/*Manda mensajes del cliente*/
int client_send(client_t* client, unsigned char* message,size_t msg_size);

/*Cierra el socket correspondiente a ese cliente*/
void client_close(client_t* client);


#endif
