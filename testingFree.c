#include "mymalloc.h"
#include <stdio.h>
#include <stdint.h>

int main(void){
    for (int i = 0; i < 8; i++){

        char *ptr = malloc(4088);
        if (ptr == NULL){
            printf("Fail: (%d) memory was not released\n",i);
            return 1;
        }
        free(ptr);
    }
        printf("PASSED: free releases memory\n");
        return 0;
}