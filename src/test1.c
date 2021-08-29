#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "mmalloc.h"

int main(int argc, char *argv[])
{
	char *array;
	int i;

	/*
	 * must be first call in the program
	 */
	InitMmalloc();

	array = Mmalloc(10);
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

	printf("here is my nifty new string: %s\n",array);

	MFree(array);

	return(0);
}
