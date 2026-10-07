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

    //freeing in forwards direction (testing backwards coalescence)
    for (int i = 0; i < OBJECTS; i++){
        free(obj[i]); 
    }

    char* mergedblock = malloc((4096-8)); // since header
    if (mergedblock == NULL){
         printf("FAIL: free chunks weren't merged \n");
         return 1;
    }
    free(mergedblock);

    // filling the heap with them
    for (int i = 0; i < OBJECTS; i++){
        obj[i] = malloc(sizeObject);
        if (obj[i] == NULL){
            printf("FAIL: cannot fill heap for some reason \n");
            return 1;
        }
    }
    //freeing in backwards direction (testing forwards coalescence)
    for (int i = OBJECTS-1; i >= 0; i--){
        free(obj[i]);
    }

    mergedblock = malloc((4096-8)); // since header
    if (mergedblock == NULL){
         printf("FAIL: free chunks weren't merged \n");
         return 1;
    }
    free(mergedblock);

    //Testing for sandwich coalescing: free outer ends first and then the center
    void *a = malloc(1000);
    void *b = malloc(1000);
    void *c = malloc(1000);

    free(a);
    free(c);
    free(b);

    mergedblock = malloc((4096-8)); // since header
    if (mergedblock == NULL){
         printf("FAIL: free chunks weren't merged \n");
         return 1;
    }
    free(mergedblock);


    
    printf("PASSED: successfully chunks merged.\n");
    return 0;
}




