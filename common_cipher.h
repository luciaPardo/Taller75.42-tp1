#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define VAL_ASCII 256

struct cipher;

typedef void (*encrypt_func_t)(struct cipher* self, unsigned char* msg,
                                                          unsigned char* buff,
                                                          size_t size_msg);
typedef void (*desencrypt_func_t)(struct cipher* self, unsigned char* msg,
                                                          unsigned char* buff,
                                                          size_t size_msg);

struct cipher{
  char* key;
  unsigned int i;
  unsigned int j;
  encrypt_func_t encrypt_func;
  desencrypt_func_t desencrypt_func;
  unsigned char* ks;
  ssize_t len_key;
  char* method;
};
/*Estructura cipher con
key --> clave cifradora de cada método
i, j --> ínidices
encrypt_func_t encrypt_func; --> puntero a función genérica de encriptar
desencrypt_func_t desencrypt_func;
ks --> keystream utilizado sólo por rc4
len_key longitud de la clave (usado por vigenere y rc4)*/


typedef struct cipher cipher_t;

/*Inicializa la estructura cifradora*/
void cipher_init(cipher_t* self, char* method, char* key);

/*Función genérica para llamar al encriptador respectivo de cada método*/
void encrypt(cipher_t* self, unsigned char* msg, unsigned char* buff,
                                                          size_t size_msg);

/*Función genérica para llamar al desencriptador respectivo de cada 
método*/                                     
void decipher(cipher_t* self, unsigned char* msg, unsigned char* buff,
                                                          size_t size_msg);

/*Función encriptadora del cifrador cesar*/
void cesar_encrypt(cipher_t* self, unsigned char* msg, unsigned char* buff,
                                                          size_t size_msg);

/*Función desencriptadora del cifrador cesar*/
void cesar_desencrypt(cipher_t* self, unsigned char* msg,unsigned char* buff,
                                                            size_t size_msg);
/*Función encriptadora del cifrador vigenere*/
void vigenere_encrypt(cipher_t* self, unsigned char* msg, unsigned char* buff,
                                                              size_t size_msg);

/*Función desencriptadora del cifrador vigenere*/
void vigenere_desencrypt(cipher_t* self, unsigned char* msg, 
                                                            unsigned char* buff,
                                                            size_t size_msg);
/*Función swap*/
void swap(unsigned char* s, unsigned int i, unsigned int j);

/*Inicializa la keystream del cifrador rc4, se invoca una vez por cifrador*/
void rc4_init(cipher_t* self, unsigned char buff[]);

/*Función encriptadora y desdencriptadora del cifrador rc4*/
void rc4_encrypt(cipher_t* self, unsigned char* msg, unsigned char* ret,
                                                              size_t size_msg);

/*En caso de ser el método rc4 se libera memoria utilizada por la keystream*/
void cipher_close(cipher_t* self);
