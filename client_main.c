
#define ARGS 5
#define HOST 1
#define PORT 2
#define METHOD 3
#define KEY 4
#define MAX_BUFF 64

#include "client.h" 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <sys/types.h>



int main(int argc, const char*  argv[]) {
    if ( ! correct_args(argc) )
        return -1;    
    if (program_init_client(argv[METHOD],argv[KEY], argv[PORT],
                                                 argv[HOST]) < 0)
        return -1;
    return 0;  
}

