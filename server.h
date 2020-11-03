#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common_socket.h" 
#include "common_cipher.h" 


struct server{
    const char* port;
    socket_t* skt;
    socket_t* client_skt;
};

typedef struct server server_t;

/*Inicializa el server */
void server_init(server_t* server, const char* port);

/*conecta el server */
int server_connect(server_t* self);

/*Permite escuchar posibles conexiones*/
bool server_listen(server_t* self, struct addrinfo* results);

/* Recibe mensajes del cliente */
int server_recv(server_t* self, unsigned char* msg, size_t msg_size);

/*Acepta la conexión de un cliente */
int server_accept_client(server_t* self);

/*Cierra el server */
void server_close(server_t* self);
/* Inicializa el programa al servidor recibiendo parámetros de main*/
int program_init_server(const char* pre_method,
                            const char* pre_key,
                            const char* port);

/* Corre el servidor recibiendo parámetros de main*/
int run_server(server_t* server, const char* port);

/* Encapsula que el servidor reciba los mensajes*/
void server_cipher_recv(server_t* server, cipher_t* cipher);

/* Valida e inicializa el cifrador*/
bool validate_and_init_cipher(const char* pre_method, const char* pre_key, 
                                                        cipher_t* cipher);
/*checkea argumentos de main*/
bool correct_args(int argc);



#endif
