
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
#define MAX_BUFF 64
#define ARGS 4
#define ARG_PORT 1
#define METHOD 2
#define KEY 3



#define MAX_CLIENTS 1

void server_init(server_t* server, const char* port){
    server->port = port;
}

int server_connect(server_t* self){
    int err = socket_server_connect(self->skt, self->port);
    if (err < 0) return -1;
    return 0;
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



int run_server(server_t* server, const char* port ){
    server_init(server, port);
    if ( server_connect(server) < 0 ){
        server_close(server);
        return -1;
    }else if ( server_accept_client(server) < 0 ){
        server_close(server);
        return -1;
    }
    return 0;
}



void server_cipher_recv(server_t* server, cipher_t* cipher){
    unsigned char buff[MAX_BUFF] = {0};
    unsigned char msg[MAX_BUFF] = {0};

    while (true){
        int new_bytes = server_recv(server, msg, MAX_BUFF);
        if ( new_bytes == 0 )
            break;
        decipher(cipher, msg, buff, new_bytes);
        fwrite(buff, 1, new_bytes , stdout);
    }
}

int program_init_server(const char* pre_method,
                            const char* pre_key,
                            const char* port){
    server_t server;
    socket_t socket;
    server.skt = &socket;   
    socket_t client_skt;
    server.client_skt = &client_skt;
    if (run_server(&server, port ) < 0) return -1;

    cipher_t cipher;
    if (!validate_and_init_cipher(pre_method, pre_key, &cipher))
        return -1;
    
    server_cipher_recv(&server, &cipher);
    cipher_close(&cipher);
    server_close(&server); 
    return 0;
}



bool validate_and_init_cipher(const char* pre_method, const char* pre_key, 
                                                        cipher_t* cipher){
    char* method;
    if ((method = strchr(pre_method, '=')) == NULL)
        return false;
    method++;
    if (strcmp(method, "cesar") && strcmp(method, "vigenere")
                                         && strcmp(method, "rc4")){
        printf("Método ingresado inválido \n");
        return false;
    }
    char* key = strchr(pre_key, '=');
    if ( key == NULL ) return false;
    key++; 
    cipher_init(cipher, method, key);
    return true;
}

bool correct_args(int argc){
    if (argc != ARGS){
        printf("Error de cantidad de parámetros \n");
        printf("Utilizar forma ./server <puerto>"); 
        printf("--method=<cifrado> --key=<clave> \n");
        return false;
    } 
    return true;
}



