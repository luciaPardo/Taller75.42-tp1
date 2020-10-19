#include "server.h" 
#include "common_socket.h" 
#include "common_cipher.h"
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

int main(int argc, const char* argv[]){

    if (argc != ARGS){
        printf("Error de cantidad de parámetros");
        printf("Utilizar forma ./server <puerto> --method=<cifrado> --key=<clave> ");
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
    }    

    else if ( server_accept_client(&server) < 0 ){
        server_close(&server);
    }
   
    char* method;
    if ((method = strchr(argv[METHOD], '=')) == NULL)
        return -1;
    method++;
    if (strcmp(method, "cesar") && strcmp(method, "vigenere") && strcmp(method, "rc4")){
        printf("Método ingresado inválido");
        return -1;
    }
    char* key = strchr(argv[KEY], '=');
    if ( key == NULL ) return -1;
    key++; 
    cipher_t cipher;
    cipher_init(&cipher, method, key);
    int new_bytes = 0;
    unsigned char buff[MAX_BUFF] = {0};
    unsigned char msg[MAX_BUFF] = {0};

    while (true){
        new_bytes = server_recv(&server, msg, MAX_BUFF);
        if( new_bytes == 0 )
            break;
        decipher(&cipher, msg, buff, new_bytes);
        fwrite(buff, 1, new_bytes , stdout);
    }
    
    if(strcmp(method, "rc4") == 0){
        cipher_close(&cipher);
    }
    server_close(&server); 
    return 0;  
}
