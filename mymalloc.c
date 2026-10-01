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


static void initialize_heap() {
    header *first_header = (header *)heap.bytes;
    //doing *heap.bytes instead would decay into a char pointer, which can't fit our header (only 1 byte)
    //this treats the address as pointing to a header struct

    first_header->is_available = 1;
    first_header->size = MEMLENGTH;

    //To do: register a leak detection function
    is_initialized = 1;
}

void *tryAllocate(size_t size_to_allocate) {
    header *boundary = (header *)(heap.bytes + MEMLENGTH);
    
    header *ptr = (header *)heap.bytes;
    while ((ptr < boundary)) {
        if ((ptr->is_available) && (ptr->size >= size_to_allocate)) {
            if ((size_to_allocate + 8) >= ptr->size) {
                ptr->is_available = 0;
                //if the chunk we take has only 8 bytes left, there's not enough space for
                //any other data there so we can just inhabit that whole chunk
                return (void *)(ptr + 1); //add 1 because we want to point to payload, not header
            }
            else {
                //otherwise we have to partition this chunk
                header *newPtr = (header *)((char *)ptr + size_to_allocate + 8);
                newPtr->is_available = 1;
                newPtr->size = (ptr->size - size_to_allocate - 8);
                //new chunk has the size of our original chunk minus space needed
                ptr->is_available = 0;
                ptr->size = size_to_allocate;
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
