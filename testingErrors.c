#include "mymalloc.h"
#include <stdio.h>
#include <stdint.h>

int main(int argc, char**argv){

    if (argc < 2){
        printf("Usage: %s <0|1|2|3|4>\n", argv[0]);
        printf("0: Free before initializing heap\n");
        printf("1: Free pointer not allocated by malloc (stack address)\n");
        printf("2: Free pointer not at the start of a chunk\n");
        printf("3: Double free\n");
        printf("4: Allocating negative number of bytes");
        return 1;
    }

    //Case 0: Calling free before initializing heap
    if (argv[1][0] == '0') {
        int x = 1;
        free(&x);
    }

    //Case 1: Calling free on an address not allocated by malloc
    if (argv[1][0] == '1'){
        int x = 0;
        char* blockToKeep = malloc(8); // need to initialize first
        blockToKeep[0] = 'x';
        free(&x); // wasn't made by malloc
    }

    //Case 2: Calling free on an address inside a chunk
    if (argv[1][0] == '2'){
        int *ptr = malloc(sizeof(int)*2);
        free(ptr+1);
    }

    //Case 3: Calling free twice on the same pointer
    if (argv[1][0] == '3'){
        int *ptr = malloc(sizeof(int)*100);
        free(ptr);
        free(ptr); // double free!!!!
    }
    
    //Case 4: Allocating negative number of bytes
    if (argv[1][0] == '4') {
        int *ptr = malloc(-4);
    }


    return 0;
}