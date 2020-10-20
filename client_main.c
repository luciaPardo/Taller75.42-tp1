
#include "common_socket.h"
#include "client.h"
#include "common_cipher.h"
#define ARGS 5
#define HOST 1
#define PORT 2
#define METHOD 3
#define KEY 4
#define MAX_BUFF 64

bool correct_args(int argc);
bool validate_and_init_cipher(cipher_t* cipher, const char* pre_method,
                                                    const char* pre_key);
int main(int argc, const char*  argv[]) {
    if ( ! correct_args(argc) ){
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
    cipher_t cipher;   
    if (!validate_and_init_cipher(&cipher, argv[METHOD], argv[KEY])){
        return -1;
    }
    unsigned char buff[MAX_BUFF] = {0};
    unsigned char msg[MAX_BUFF] = {0};
    while (!feof(stdin)){
        size_t result = fread(msg, 1,  MAX_BUFF, stdin);
        encrypt(&cipher,msg, buff, result);
        int bytes_sent = client_send(&client, buff, result);
        if ( bytes_sent < 0 || result == 0)
            break;
    }
    cipher_close(&cipher);
    client_close(&client); 
    return 0;  
}

bool validate_and_init_cipher(cipher_t* cipher, const char* pre_method, 
                                                        const char* pre_key){
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

