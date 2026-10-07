#include "mymalloc.h"
#include <stdio.h>
#include <stdint.h>



int main(void){

    char* block = malloc(5000); // too big
    if (block != NULL){
        printf("FAIL: malloc(5000) should have returned NULL\n");
        return 1;
    }

    block = malloc(4089); // too big (after padding)
    if (block != NULL){
        printf("FAIL: malloc(4089) should return null since padding/rounding\n");
        return 1;
    }

    block = malloc(4088); 
    if (block == NULL){
        printf("FAIL: malloc(4088) should've succeeded\n");
        return 1;
    }
    free(block);
    printf("PASSED: malloc behaved normally.");
    return 0;
}