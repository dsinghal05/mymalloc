#include "mymalloc.h"
#define MEMLENGTH 4096


static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;
static int is_initialized = 0;


static void initialize_heap() 
{
    //To Do: add a header that marks all space as free
    is_initialized = 1;
}


void *mymalloc(size_t size, char *file, int line)
{
    if (!is_initialized) 
    {
        initialize_heap();
    }
    return nullptr;
}

void   myfree (void *ptr, char *file, int line)
{

}
