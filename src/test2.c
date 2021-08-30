#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/mmalloc.h"

int main(int argc, char *argv[])
{
	char *a1;
	char *a2;
	char *a3;
	char *a4;

	/*
	 * must be first call in the program
	 */
	InitMmalloc();

	a1 = (char *)Mmalloc(128);
	if(a1 == NULL)
	{
		fprintf(stderr,"call to Mmalloc(128) failed\n");
		fflush(stderr);
		exit(1);
	}

	printf("FREE LIST after malloc(128)\n");
	PrintMmallocFreeList();

	a2 = (char *)Mmalloc(32);
	if(a2 == NULL)
	{
		fprintf(stderr,"first call to Mmalloc(32) failed\n");
		fflush(stderr);
		exit(1);
	}

	printf("FREE LIST after malloc(32)\n");
	PrintMmallocFreeList();

	MFree(a1);

	printf("FREE LIST after free of first 128 malloc()\n");
	PrintMmallocFreeList();

	a3 = (char *)Mmalloc(104);
	if(a3 == NULL)
	{
		fprintf(stderr,"call to Mmalloc(104) failed\n");
		fflush(stderr);
		exit(1);
	}

	printf("FREE LIST after malloc(104)\n");
	PrintMmallocFreeList();

	a4 = (char *)Mmalloc(8);
	if(a4 == NULL)
	{
		fprintf(stderr,"call to Mmalloc(8) failed\n");
		fflush(stderr);
		exit(1);
	}
	printf("FREE LIST after malloc(8)\n");
	PrintMmallocFreeList();

	/*
	 * free it all -- notice that a1 is already free
	 */
	MFree(a2);
	MFree(a3);
	MFree(a4);
	printf("FREE LIST after all free\n");
	PrintMmallocFreeList();


	return(0);
}
