
#include "common_socket.h"
#include "client.h"
#include "common_cipher.h"
#define ARGS 5
#define HOST 1
#define PORT 2
#define METHOD 3
#define KEY 4
#define MAX_BUFF 64

int main(int argc, const char*  argv[]) {
    if (argc != ARGS){
		printf("%d  \n", argc);
        printf("Error de cantidad de parámetros \n");
        printf("Utilizar forma ./client <client-host>");
        printf(" <client-port> --method=<method> --key=<key>");
        return -1;
    } 
    client_t client;
    socket_t socket;
    client.skt = &socket;

    client_init(&client, argv[HOST], argv[PORT]);
    if ( client_connect(&client) < 0 ){
        printf("Error conectando el cliente \n");
        client_close(&client);
        return -1;
    }    
    
    char* method;
    
    if ((method = strchr(argv[METHOD], '=')) == NULL){
        printf("Error de entrada de argumento method \n");
        return -1;
    }
     
    method++;

    if (strcmp(method, "cesar") && strcmp(method, "vigenere")
                                                && strcmp(method, "rc4")){
        printf("Método ingresado inválido \n");
        return -1;
    }
  
    char* key = strchr(argv[KEY], '=');
    if (key == NULL ) return -1;
    key++; 
    cipher_t cipher;
    cipher_init(&cipher, method, key);
    unsigned char buff[MAX_BUFF] = {0};
    unsigned char msg[MAX_BUFF] = {0};
    while (!feof(stdin)){
        size_t result = fread(msg, 1,  MAX_BUFF, stdin);
        encrypt(&cipher,msg, buff, result);
        int bytes_sent = client_send(&client, buff, result);
        if ( bytes_sent < 0 || result == 0)
            break;
    }
    if (strcmp(method, "rc4") == 0){
        cipher_close(&cipher);
    } 
    client_close(&client); 
    return 0;  
}

