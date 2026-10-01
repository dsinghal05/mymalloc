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
    return NULL;
}


void *mymalloc(size_t size, char *file, int line)
{
    if (!is_initialized) 
    {
        initialize_heap();
    }
    size = (size+7) & ~7; //rounds up to 8 byte multiple

    void *returnptr = tryAllocate(size);
    //return a pointer to THE PAYLOAD of an allocated chunk (so after the header)
    return returnptr;
}

void   myfree (void *ptr, char *file, int line)
{

}
