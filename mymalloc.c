#include "mymalloc.h"
#include <stdio.h>
#include <stdint.h>
#define MEMLENGTH 4096


static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;
static int is_initialized = 0;

typedef struct {
    int is_available;
    uint32_t size;
    //both fields are 4 bytes long meaning each header is in total 8 bytes
} header;

static header *findNextChunk(header *chunk){
    return chunk + chunk ->size / 8;
}

static void findLeaks(void){
 header *end = (header*)(heap.bytes + MEMLENGTH);
 header *ptr = (header*)(heap.bytes);

 int count = 0;
 int totalAmt = 0;
 while (ptr < end){
    if (!((*ptr).is_available)){
        count++;
        totalAmt += (*ptr).size - 8;
    }
    ptr = findNextChunk(ptr);
 }
 if (count > 0) {
    fprintf(stderr, "mymalloc: %d bytes leaked in %d objects.\n", totalAmt, count);
 }
}


static void initialize_heap() {
    header *first_header = (header *)heap.bytes;
    //doing *heap.bytes instead would decay into a char pointer, which can't fit our header (only 1 byte)
    //this treats the address as pointing to a header struct

    first_header->is_available = 1;
    first_header->size = MEMLENGTH;

  
    is_initialized = 1;
    atexit(findLeaks); // run findLeaks when program ends
}




void *tryAllocate(size_t size_to_allocate) {
    header *boundary = (header *)(heap.bytes + MEMLENGTH);
    
    header *ptr = (header *)heap.bytes;
    while ((ptr < boundary)) {
        if ((ptr->is_available) && (ptr->size >= size_to_allocate+8)) {
            size_t leftover = ptr->size - (size_to_allocate+8);
             if (leftover < 16) {
                    ptr-> is_available = 0;
                    return (void*)(ptr+1);
            }
            else {
                //otherwise we have to partition this chunk
                header *newPtr = (header *)((char *)ptr + size_to_allocate + 8);
                newPtr->is_available = 1;
                newPtr->size = (ptr->size - size_to_allocate - 8);
                //new chunk has the size of our original chunk minus space needed
                ptr->is_available = 0;
                ptr->size = size_to_allocate+8;
                return (void *)(ptr + 1); //add 1 because we want to point to payload, not header
            }
        }
        ptr = (header *)((char *)ptr + ptr->size);
        //traverse to next header in linked-list style
    }
    //if we reach here then there's no available chunks with adequate size
    //To do: print message to standard error
    return NULL; // So caller handles it
}


void *mymalloc(size_t size, char *file, int line)
{
    if (!is_initialized) 
    {
        initialize_heap();
    }
    size_t requestedSize = size;
    size = (size+7) & ~7; //rounds up to 8 byte multiple

    void *returnptr = tryAllocate(size);
    if (returnptr == NULL){
        fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", requestedSize, file, line);
    }
    //return a pointer to THE PAYLOAD of an allocated chunk (so after the header)
    return returnptr;
}

static void coalesce (header *chunk1, header *chunk2) {
    chunk1->size += chunk2->size;
}

void   myfree (void *ptr, char *file, int line)
{
    if (!is_initialized) {
        // no initialization needed
        //throw error - nothing to free as heap was not yet initialized
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2);
    }
    
    //check to make sure ptr is within heap
    if (((char *)ptr < heap.bytes + 8) || ((char *) ptr > heap.bytes + MEMLENGTH)) {
        //throw error - out of bounds
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2);
    }

    header *ptr1 = (header *)heap.bytes;
    header *prev = NULL;
    while ((char*)ptr1 + 8 < (char *)ptr) { //ptr points to beginning of payload, not to header
        prev = ptr1;
        ptr1 = (header *)((char*)ptr1 + ptr1->size);
    }

    if ((char *)ptr1 + 8 == (char *)ptr) {
        //ptr1 points to start of header, therefore ptr refers to valid payload.
        if (ptr1->is_available) {
            //throw error - don't free something twice
            fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
            exit(2);
        }
        ptr1->is_available = 1;

        //check for coalesence:
        header *forward = (header *)((char *)ptr1 + ptr1->size);
        if (((char *) forward < heap.bytes + MEMLENGTH) && (forward->is_available)) {
            coalesce(ptr1, forward);
        }
        if ((prev != NULL) && (prev->is_available)) {
            //check to make sure its not NULL, in case ptr refers to the first chunk
            coalesce(prev, ptr1);
        }
    }
    
    else {
        //throw error, ptr is invalid
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2);
    }
}
