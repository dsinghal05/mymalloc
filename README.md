Authors: Divit Singhal (ds2234), Richi Patel (rbp98)

mymalloc()
-----------
This project implements a dynamic memory allocator replacing standard library malloc() and free() on a simulated memory heap. The design models the heap as a sequence of variably sized chunks, where a chunk is a continuous sequence of bytes in the heap. 

Metadata Structure: Each chunk contains an 8 byte header positioned directly before the payload, which stores whether the chunk is free or in-use and the total chunk size (payload + header). This information allows a mymalloc() call to traverse the heap in a linked list style by traversing the headers to find the first available chunk with a sufficiently sized payload. 

Alignment: All requested sizes are rounded up to the nearest multiple of 8 to ensure memory alignment is satisfied. 

Chunk splitting: When an available chunk is found that has a size greater than the requested allocation by at least 16 bytes (8 for header + 8 for payload), the chunk is split and the leftover space is converted to a new available chunk with its own header. 

myfree()
---------
Whenever an object is deallocated, myfree() tests for forward and backwards coalescing so adjacent free blocks are always coalesced. 
Forward Coalescing: Checks if the contiguous forward chunk is within heap boundaries and free. If so, their sizes are merged.
Backward Coalescing: A prev pointer trails one chunk behind the target address. If prev is free, it absorbs the newly freed block (and any forward chunk it already merged with).
These two systems work together to ensure that if both the preceding and following chunks are free, all three are combined into one contiguous chunk. 

Error Handling and Detection
-----------------------------
The program detects invalid operations and provides error messages, and exits with status code 2. 
-Rejects mymalloc() calls requesting 0 or negative bytes
-Rejects free calls made prior to heap initialization
-Verifies pointers passed to free fall within bounds of heap
-Ensures pointers passed to free match the exact start of a payload
-Rejects attempts to free memory if already available
The program reports and remaining allocated chunks and total leaked bytes at exit. 

Correction Testing
---------------------
The program is verified using 5 dedicated test programs:
**`testingCoalesce.c`**: Verifies all three coalescing pathways (backward, forward, and bidirectional).
**`testingErrors.c`**: Validates robust detection and error handling for invalid operations.
**`testingFree.c`**: Validates memory reclamation over repeated allocation cycles.
**`testingLeak.c`**: Confirms automatic memory leak detection.
**`testingMallocFail.c`**: Tests boundary conditions and graceful failure handling.

Stress Testing and Benchmark
-----------------------------
memgrind.c benchmarks the program across 50 iterations of 5 workloads, timed with gettimeofday().
Task 1: Allocates and frees blocks of increasing power-of-two sizes (8, 16, 32, ..., 1024 bytes) in reverse order.   
Task 2: Allocates 120 small 8-byte objects sequentially, then frees all 120 sequentially.   
Task 3: Alternates randomly between allocating 1-byte objects and freeing an arbitrary active object until 120 allocations occur, then drains all remaining blocks.   
Task 4: Allocates 100 blocks of random sizes and deallocates them in randomized pairwise order.
Task 5: Creates a "checkerboard" fragmentation pattern by allocating small blocks and freeing all even-indexed chunks, then freeing odd-indexed chunks to stress bidirectional coalescing.
Average time for workload: ~0.120 ms