
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
#define ARGS 5
#define HOST 1
#define PORT 2
#define METHOD 3
#define KEY 4
#define MAX_BUFF 64




void client_init(client_t* client, const char* host, const char* port){
    client->host = host;
    client->port = port;
    socket_init(client->skt);
}

int client_connect(client_t* client){
    int r = socket_client_connect(client->skt, client->host, client->port);
    if (r < 0) return r;
    return r;
}

int client_send(client_t* client, unsigned char* msg, size_t msg_size){
    return socket_send(client->skt, msg, msg_size);
}  

void client_close(client_t* client){
    if (!client) return;
    socket_close(client->skt);
}


int program_init_client(const char* pre_method,
                            const char* pre_key,
                            const char* port,
                            const char* host){
    client_t client;
    socket_t socket;
    client.skt = &socket;
    if (run_client(&client, port, host ) < 0) return -1;
    cipher_t cipher;
    if (!validate_and_init_cipher(pre_method, pre_key, &cipher))
        return -1;
    client_cipher_send(&client, &cipher);
    cipher_close(&cipher);
    client_close(&client); 
    return 0;
}


int run_client(client_t* client, const char* port, const char* host){
    client_init(client, host, port);
    if ( client_connect(client) < 0 ){
        printf("Error conectando el cliente \n");
        client_close(client);
        return -1;
    } 
    return 0;
}

void client_cipher_send(client_t* client, cipher_t* cipher){
    unsigned char buff[MAX_BUFF] = {0};
    unsigned char msg[MAX_BUFF] = {0};
    while (!feof(stdin)){
        size_t result = fread(msg, 1,  MAX_BUFF, stdin);
        encrypt(cipher,msg, buff, result);
        int bytes_sent = client_send(client, buff, result);
        if ( bytes_sent < 0 || result == 0)
            break;
    }
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
        printf("Utilizar forma ./client <host> <puerto>"); 
        printf("--method=<cifrado> --key=<clave> \n");
        return false;
    } 
    return true;
}


