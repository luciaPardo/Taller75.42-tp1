#include "server.h" 
#include "common_socket.h" 
#include "common_cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <sys/types.h>
#define MAX_BUFF 64
#define ARGS 4
#define ARG_PORT 1
#define METHOD 2
#define KEY 3

bool validate_and_init_cipher(const char* pre_method, const char* pre_key, 
                                                        cipher_t* cipher);
bool correct_args(int argc);

int main(int argc, char* argv[]){
    if ( ! correct_args(argc) ){
        return -1;
    } 
    server_t server;
    socket_t socket;
    server.skt = &socket;   
    socket_t client_skt;
    server.client_skt = &client_skt;
    server_init(&server, argv[ARG_PORT]);
    
    if ( server_connect(&server) < 0 ){
        server_close(&server);
        return -1;
    }else if ( server_accept_client(&server) < 0 ){
        server_close(&server);
        return -1;
    }
    cipher_t cipher;
    if (!validate_and_init_cipher(argv[METHOD], argv[KEY], &cipher)){
        return -1;
    }
    unsigned char buff[MAX_BUFF] = {0};
    unsigned char msg[MAX_BUFF] = {0};

    while (true){
        int new_bytes = server_recv(&server, msg, MAX_BUFF);
        if ( new_bytes == 0 )
            break;
        decipher(&cipher, msg, buff, new_bytes);
        fwrite(buff, 1, new_bytes , stdout);
    }
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


