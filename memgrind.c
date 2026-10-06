#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

void taskOne() {
    void *ptr8 = malloc(8);
    void *ptr16 = malloc(16);
    void *ptr32 = malloc(32);
    void *ptr64 = malloc(64);
    void *ptr128 = malloc(128);
    void *ptr512 = malloc(512);
    void *ptr1024 = malloc(1024);

    free(ptr1024);
    free(ptr512);
    free(ptr128);
    free(ptr64);
    free(ptr32);
    free(ptr16);
    free(ptr8);
}

void taskTwo() {
    void *ptrArray[120];
    for (int i = 0; i < 120; i++) {
        ptrArray[i] = malloc(8);
    }
    for (int i = 0; i < 120; i++) {
        free(ptrArray[i]);
    }
}

void taskThree() {
    void *ptrArray[120];
    int allocCount = 0; //number of active allocations
    int totalAllocations = 0;

    while (totalAllocations < 120) {
        int random = rand() % 2;
        if (random == 0) {
            // Allocate a one byte object
            ptrArray[allocCount] = malloc(1);
            allocCount++;
            totalAllocations++;
        }
        if (random == 1) {
            // Free (if possible)
            if (allocCount > 0) {
                int randomInd = rand() % allocCount;
                free(ptrArray[randomInd]);
                ptrArray[randomInd] = ptrArray[allocCount-1]; //swaps last element into this index
                allocCount--;
            }
        }
    }
    while (allocCount > 0) {
        //deallocate all remaining objects
        free(ptrArray[allocCount-1]);
        allocCount--;
    }
}

void taskFour() {
    void *ptrArray[100];
    
    for (int i = 0; i < 100; i++) {
        int size = (rand() % 3) + 1;
        ptrArray[i] = malloc(size * 8);
    }
    for (int i = 0; i < 99; i += 2) {
        int coin = rand() % 2;
        if (coin == 1) {
            free(ptrArray[i+1]);
            free(ptrArray[i]);
        }
        else {
            free(ptrArray[i]);
            free(ptrArray[i+1]);
        }
    }
}

void taskFive() {
    void *ptrArray[100];

    for (int i = 0; i < 100; i++) {
        ptrArray[i] = malloc(24);
    }

    for (int i = 0; i < 100; i += 2) {
        free(ptrArray[i]);
    }

    for (int i = 1; i < 100; i += 2) {
        free(ptrArray[i]);
    }

    void *largeBlock = malloc(3000);
    free(largeBlock);
}

int main(void) {

    //This program will use gettimeofday() to get the starting time, execute the workload 50 times, 
    //get the ending time, and then report the average time for the workload 
    // (computed by dividing the elapsed time by the number of runs).
    struct timeval start, end;

    // Record start time
    gettimeofday(&start, NULL);

    // Perform benchmark
    for (int i = 0; i < 50; i++) {
        taskOne();
        taskTwo();
        taskThree();
        taskFour();
        taskFive();
    }

    // Record end time
    gettimeofday(&end, NULL);

    double start_time = start.tv_sec + (start.tv_usec / 1000000.0);
    double end_time   = end.tv_sec   + (end.tv_usec   / 1000000.0);

    double elapsed_sec = end_time - start_time;
    elapsed_sec /= 50;

    printf("Average time for workload: %.3f ms\n", elapsed_sec*1000);

}