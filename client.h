#ifndef CLIENT_H
#define CLIENT_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <netdb.h>
#include "common_socket.h"
#include "common_cipher.h"


typedef struct client{
    const char* host;
    const char* port;
    socket_t* skt;
} client_t;

//typedef struct cipher cipher_t;

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

/* Inicializa el programa al cliente recibiendo parámetros de main*/
int program_init_client(const char* pre_method,
                            const char* pre_key,
                            const char* port,
                            const char* host);

/* Encapsula que el cliente mande y encripte los mensajes*/
void client_cipher_send(client_t* client, cipher_t* cipher);

/* Corre el cliente recibiendo parámetros de main*/
int run_client(client_t* client, const char* port, const char* host);

/*valida e inicializa el cifrador*/
bool validate_and_init_cipher(const char* pre_method, const char* pre_key, 
                                                        cipher_t* cipher);
/*checkea argumentos del main*/
bool correct_args(int argc);


#endif
