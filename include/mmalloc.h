#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//TODO: implementirati algoritme dlmalloc-a
//TODO: implementirati druge verzije malloca: jemalloc, ptmalloc, dlmalloc, SegmentHeapWindows, NT Heap, kmallocKernelLinux, kallocIOS; pa je moguce specificirati koji neko zeli

#ifndef M_MALLOC_H
#define M_MALLOC_H

#define MAX_MALLOC_SIZE (1024*1024*16) //16MB heap
#define MALLOC_CHUNK_REQUEST (1024) //16KB should be large enough

//instead of using Struct only, with union i can ensure alignment to 16B with long
typedef unsigned int uint;

union header
{
	struct
	{
		//Since the headers are aligned, lowest bit will always be zero
		//0x00000000	00-ti header
		//0x00010000	16-ti header
		//0x00100000	32-i  header
		//0x00110000	48-i  header...
		//sledi da ce uvek poslednji bit biti 0
		//pa moze da se koristi za cuvanje informacije dal je free ili inuse
		uint size;	
		union header *next_blck;
	} s;
	long int x;
};

typedef union header Header;

void InitMmalloc();
void *Mmalloc(size_t size);
void MFree(void *buffer);
void PrintMmallocFreeList();

int retsize(uint size);
int uorf(uint size);
Header *memadd(size_t size);

#endif /* M_MALLOC_H */
