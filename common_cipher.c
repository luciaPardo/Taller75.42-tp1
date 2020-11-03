#include "common_cipher.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>


#define CHUNK_SIZE 64
#define MAX_BUFF 100
#define VAL_ASCII 256

void cipher_init(cipher_t* self,char* method, char* key){
    self->i = 0;
    self->j = 0;
    self->key = key;
    self->method = method;
  if (strcmp(method, "cesar") == 0){
    self->encrypt_func = cesar_encrypt;
    self->desencrypt_func = cesar_desencrypt;
  }

  if (strcmp(method, "vigenere") == 0){
    self->len_key = strlen(self->key);
    self->encrypt_func = vigenere_encrypt;
    self->desencrypt_func = vigenere_desencrypt;
  }
  
  if (strcmp(method, "rc4") == 0){ 
    self->len_key = strlen(self->key);   
    self->ks =  malloc(VAL_ASCII * sizeof(unsigned char));
    rc4_init(self, self->ks);
    self->encrypt_func = rc4_encrypt;
    self->desencrypt_func = rc4_encrypt;
  }
}


void encrypt(cipher_t* self, unsigned char* msg, 
                                                unsigned char* buff,
                                              size_t size_msg){
  self->encrypt_func(self, msg, buff, size_msg);
}


void decipher(cipher_t* self, unsigned char* msg, unsigned char* buff,
                                                          size_t size_msg){
  self->desencrypt_func(self, msg, buff, size_msg);
}


void cesar_encrypt(cipher_t* self, unsigned char* msg, 
                                                      unsigned char* buff,
                                                      size_t size_msg){
  int offset;                                            
  if ((offset = atoi(self->key)) == 0){
    printf("Error offset método cesar %d \n", offset);
    return;
  }
  for (self->i = 0; self->i < size_msg; self->i++){   
    buff[self->i] = (msg[self->i] + offset) % VAL_ASCII; 
  }
}

void cesar_desencrypt(cipher_t* self, unsigned char* msg,
                                                        unsigned char* buff,
                                                        size_t size_msg){
  int offset;
  if ((offset = atoi(self->key)) == 0){
    return;
  }
  for (self->i = 0; self->i < size_msg; self->i++){  
      buff[self->i] = (msg[self->i] - offset) % VAL_ASCII;
  }
}

void vigenere_encrypt(cipher_t* self, unsigned char* msg, 
                                                        unsigned char* buff,
                                                        size_t size_msg){
  for (self->j = 0; self->j < size_msg ; self->i++, self->j++){
      self->i = self->i % self->len_key;
      buff[self->j] = (msg[self->j] + self->key[self->i]) % VAL_ASCII;
  }
}

void vigenere_desencrypt(cipher_t* self, unsigned char* msg, 
                                                        unsigned char* buff,
                                                        size_t size_msg){
  for (self->j = 0; self->j < size_msg ; self->i++, self->j++){
      self->i = self->i % self->len_key;
      buff[self->j] = (msg[self->j] - self->key[self->i]) % VAL_ASCII;
  }
}


void swap(unsigned char* s, unsigned int i, unsigned int j) {
    unsigned char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}

void rc4_init(cipher_t* self,unsigned char buff[]){
    for (self->i = 0; self->i < VAL_ASCII; self->i++)
        buff[self->i] = self->i;
    for (self->i = self->j = 0; self->i < VAL_ASCII; self->i++){
        self->j = (self->j + self->key[self->i % self->len_key] + buff[self->i])
                                                                % VAL_ASCII;
        swap(buff, self->i, self->j);
    }
    self->i = 0;
    self->j = 0;
}

void rc4_encrypt(cipher_t* self, unsigned char* msg, unsigned char* ret,
                                                        size_t size_msg){
    for (int k = 0; k < size_msg ; k++){
      self->i = (self->i + 1) % VAL_ASCII;
      self->j = (self->j + self->ks[self->i]) % VAL_ASCII;
      swap(self->ks, self->i, self->j);
      int rnd = self->ks[(self->ks[self->i] + self->ks[self->j]) % VAL_ASCII];
      ret[k] = msg[k] ^ rnd;
    } 
}

void cipher_close(cipher_t* self){
  if (strcmp(self->method, "rc4") == 0)
    free(self->ks);
}

#ifdef PRUEBAS
#endif
