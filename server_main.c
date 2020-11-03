
#include "server.h" 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define MAX_BUFF 64
#define ARGS 4
#define ARG_PORT 1
#define METHOD 2
#define KEY 3


int main(int argc, char* argv[]){
    if ( ! correct_args(argc) ){
        return -1;
    } 
    if (program_init_server(argv[METHOD],argv[KEY], argv[ARG_PORT]) < 0)
        return -1;
    return 0;  
}




