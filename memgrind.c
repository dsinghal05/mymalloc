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
    int arrayCount = -1;
    int totalAllocations = 0;

    while (totalAllocations < 120) {
        int random = rand() % 2;
        if (random == 0) {
            // Allocate a one byte object
            arrayCount++;
            ptrArray[arrayCount] = malloc(1);
            totalAllocations++;
        }
        if (random == 1) {
            // Free (if possible)
            if (arrayCount > -1) {
                free(ptrArray[arrayCount]);
                arrayCount--;
            }
        }
    }
    while (arrayCount > -1) {
        //deallocate all remaining objects
        free(ptrArray[arrayCount]);
        arrayCount--;
    }
}

void taskFour() {
    void *ptrArray[150];
    for (int i = 0; i < 50; i++) {
        ptrArray[i] = malloc(8);
    }
    for (int i = 50; i < 100; i++) {
        ptrArray[i] = malloc(16);
    }
    for (int i = 100; i < 150; i++) {
        ptrArray[i] = malloc(24);
    }
    for (int i = 149; i > -1; i--) {
        free(ptrArray[i]);
    }
}

void taskFive() {
    //to do
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

    printf("Average time for workload: %.3f ms (%.6f seconds)\n", elapsed_sec*1000, elapsed_sec);

}