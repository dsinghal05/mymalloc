#include "mymalloc.h"
#include <stdio.h>
#include <stdint.h>


#define OBJECTS 64
#define sizeObject 56

int main(void) {
    char *obj[OBJECTS];
    // filling the heap with them
    for (int i = 0; i < OBJECTS; i++){
        obj[i] = malloc(sizeObject);
        if (obj[i] == NULL){
            printf("FAIL: cannot fill heap for some reason \n");
            return 1;
        }
    }

    for (int i = 0; i < OBJECTS; i++){
        free(obj[i]); // freeing all the objects now
    }

    char* mergedblock = malloc((4096-8)); // since header
    if (mergedblock == NULL){
         printf("FAIL: free chunks weren't merged \n");
         return 1;
    }
    free(mergedblock);
    printf("PASSED: successfully chunks merged.\n");
    return 0;
}




