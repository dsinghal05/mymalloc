CC = gcc
CFLAGS = -std=c99 -Wall -g

all: memgrind memtest testingCoalesce testingFree testingErrors testingLeak testingMallocFail

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) -o memgrind memgrind.o mymalloc.o

memtest: memtest.c mymalloc.o mymalloc.h
	$(CC) $(CFLAGS) -o memtest memtest.c mymalloc.o

testingCoalesce: testingCoalesce.c mymalloc.o mymalloc.h
	$(CC) $(CFLAGS) -o testingCoalesce testingCoalesce.c mymalloc.o

testingFree: testingFree.c mymalloc.o mymalloc.h
	$(CC) $(CFLAGS) -o testingFree testingFree.c mymalloc.o

testingErrors: testingErrors.c mymalloc.o mymalloc.h
	$(CC) $(CFLAGS) -o testingErrors testingErrors.c mymalloc.o

testingLeak: testingLeak.c mymalloc.o mymalloc.h
	$(CC) $(CFLAGS) -o testingLeak testingLeak.c mymalloc.o

testingMallocFail: testingMallocFail.c mymalloc.o mymalloc.h
	$(CC) $(CFLAGS) -o testingMallocFail testingMallocFail.c mymalloc.o

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

clean:
	rm -f *.o memgrind memtest testingCoalesce testingFree testingErrors testingLeak testingMallocFail