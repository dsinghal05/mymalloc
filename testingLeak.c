#include "mymalloc.h"
#include <stdio.h>
#include <stdint.h>


int main(void){
    char *leak = malloc(20); // not freeing, rounds up to 24 bytes
    char *frees = malloc(100);
    leak[0] = 'a'; // just because i dont want compiler errors for not using it
    free(frees);
    return 0;

}