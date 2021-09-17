#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/mmalloc.h"

int main(int argc, char *argv[])
{
	char *array;
	int i;
	int s = argc>1?atoi(argv[1]):10;
	
	/*
	 * must be first call in the program
	 */
	InitMmalloc();

	array = Mmalloc(s);
	
	if(array == NULL)
	{
		fprintf(stderr,"call to Mmalloc() failed\n");
		fflush(stderr);
		exit(1);
	}

	for(i=0; i < 9; i++)
	{
		array[i] = 'a' + i;
	}
	array[9] = 0;
	
	PrintMmallocFreeList();
	printf("here is my nifty new string: %s\n",array);

	MFree(array);

	return(0);
}
