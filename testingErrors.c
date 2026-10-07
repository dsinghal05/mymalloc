#include "mymalloc.h"
#include <stdio.h>
#include <stdint.h>

int main(int argc, char**argv){

    if (argc < 2){
        printf("1,2,3 \n");
        return 1; // So i dont have to make 3 separate files for each case
    }

    if (argv[1][0] == '1'){
        int x = 0;
        char* blockToKeep = malloc(8); // need to initialize first
        blockToKeep[0] = 'x';
        free(&x); // wasn't made by malloc
    }

    if (argv[1][0] == '2'){
        int *ptr = malloc(sizeof(int)*2);
        free(ptr+1);
    }

    if (argv[1][0] == '3'){
        int *ptr = malloc(sizeof(int)*100);
        free(ptr);
        free(ptr); // double free!!!!
    }


    return 0;
}