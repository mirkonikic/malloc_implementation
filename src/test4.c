#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/mmalloc.h"

int main(int argc, char *argv[])
{
	int size;
	void *ptr[10];
	int i;

	InitMmalloc();
	/*
	 * try mallocing four pieces, each 1/4 of total size
	 */
	size = MAX_MALLOC_SIZE / 4;

	ptr[0] = Mmalloc(size);
	if(ptr[0] == NULL)
	{
		printf("malloc of ptr[0] failed for size %d\n",
				size);
		exit(1);
	}

	PrintMmallocFreeList();
	printf("\n");

	ptr[1] = Mmalloc(size);
	if(ptr[1] == NULL)
	{
		printf("malloc of ptr[1] failed for size %d\n",
				size);
		exit(1);
	}

	PrintMmallocFreeList();
	printf("\n");

	ptr[2] = Mmalloc(size);
	if(ptr[2] == NULL)
	{
		printf("malloc of ptr[2] failed for size %d\n",
				size);
		exit(1);
	}

	PrintMmallocFreeList();
	printf("\n");

	/*
	 * this one should fail due to rounding
	 */
	ptr[3] = Mmalloc(size);
	if(ptr[3] == NULL)
	{
		printf("malloc of ptr[3] fails correctly for size %d\n",
				size);
	}

	PrintMmallocFreeList();
	printf("\n");

	/*
	 * free the first block
	 */
	MFree(ptr[0]);

	PrintMmallocFreeList();
	printf("\n");

	/*
	 * free the third block
	 */
	MFree(ptr[2]);

	PrintMmallocFreeList();
	printf("\n");

	/*
	 * now free second block
	 */
	MFree(ptr[1]);

	PrintMmallocFreeList();
	printf("\n");

	/*
	 * re-malloc first pointer
	 */
	ptr[0] = Mmalloc(size);
	if(ptr[0] == NULL)
	{
		printf("re-malloc of ptr[0] failed for size %d\n",
				size);
		exit(1);
	}
	PrintMmallocFreeList();
	printf("\n");

	/*
	 * try splitting the second block
	 */
	ptr[1] = Mmalloc(size/2);
	if(ptr[1] == NULL)
	{
		printf("split second block ptr[1] failed for size %d\n",
				size/2);
		exit(1);
	}
	PrintMmallocFreeList();
	printf("\n");

	/*
	 * free first block and split of second
	 */
	MFree(ptr[0]);
	MFree(ptr[1]);

	PrintMmallocFreeList();
	printf("\n");

	/*
	 * try mallocing a little less to make sure no split occurs
	 * first block from previous print should not be split
	 */
	ptr[0] = Mmalloc(size-1);
	if(ptr[0] == NULL)
	{
		printf("slightly smaller malloc of ptr[0] failed for size %d\n",
				size);
		exit(1);
	}

	/*
	 * free it and make sure it comes back as the correct size
	 */
	MFree(ptr[0]);
	
	PrintMmallocFreeList();
	printf("\n");

	/*
	 * okay, now see if multiples work
	 */
	for(i=0; i < 6; i++)
	{
		ptr[i] = Mmalloc(100);
	}

	/*
	 * free first block, third block, fifth block
	 */
	MFree(ptr[0]);
	MFree(ptr[2]);
	MFree(ptr[4]);
	PrintMmallocFreeList();
	printf("\n");

	/*
	 * now, free second block -- first, second, third blocks
	 * should coalesce
	 */
	MFree(ptr[1]);
	PrintMmallocFreeList();
	printf("\n");

	/*
	 * free the sixth block and it should merge with the last
	 * block leaving two
	 */
	MFree(ptr[5]);
	PrintMmallocFreeList();
	printf("\n");

	/*
	 * now free fourth block and they should all be together
	 */
	MFree(ptr[3]);
	PrintMmallocFreeList();
	printf("\n");

	printf("made it -- passed test\n");

	exit(0);
}
