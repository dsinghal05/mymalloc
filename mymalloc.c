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


static void initialize_heap() 
{
    header *first_header = (header *)heap.bytes;
    //doing *heap.bytes instead would decay into a char pointer, which can't fit our header (only 1 byte)
    //this treats the address as pointing to a header struct

    first_header->is_available = 1;
    first_header->size = MEMLENGTH;

    is_initialized = 1;
}


void *mymalloc(size_t size, char *file, int line)
{
    if (!is_initialized) 
    {
        initialize_heap();
    }
    size = (size+7) & ~7; //rounds up to 8 byte multiple

    //return a pointer to THE PAYLOAD of an allocated chunk (so after the header)
    return nullptr;
}

void   myfree (void *ptr, char *file, int line)
{

}
