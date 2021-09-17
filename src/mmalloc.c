#include "../include/mmalloc.h"

//IMPLICIT LIST TYPE : headers keep track if the block is used
//EXPLICIT LIST TYPE : only free blocks are pointed to
//SEGREGATED FREE LIST : there is separate free blocks list
//BLOCK SORTED BY SIZE : red_black tree with pointers to each free block and size used as key

//For now i chose implicit list type

//FIRST FIT : Always start from beginning of list and find first block
//NEXT FIT : Continute where you left off, for that reason i use pi as a global pointer
//BEST FIT : Tries to find supremum of available free blocks

//For now i will use next fit

static Header base;
static Header *lbp = &base;	//List Base Pointer
static Header *fbp = NULL;	//Free Block Pointer

void InitMmalloc()
{
	printf("	MMALLOC		\n");
	printf("		by mirko\n");
	printf("		        \n");
	printf("sizeof(int): %ld\n", sizeof(int));
	printf("sizeof(Header): %ld\n\n", sizeof(Header));
	return;
}

//Used for memory allocation
void *Mmalloc(size_t size)
{
	Header *i, *pi;
	uint nsegments;	//Size of headers, since one block is the size of header
	
	nsegments = (size+sizeof(Header)-1)/sizeof(Header) + 1;
	//printf("0. number of segments needed: %d for %ld bytes\n", nsegments, size);

	//Check if there is no list
	if((pi=fbp)==NULL)
	{
		lbp->s.next_blck=pi=fbp=lbp;	//Set values for base header of the list
		lbp->s.size=0;			//Set values for base header of the list
	}

	//loop trough all blocks
	for(i = pi->s.next_blck ; ; pi = i, i = i->s.next_blck)
	{
		//Check if the block is bigger or equal to the requested size
		if(i->s.size/sizeof(Header) >= nsegments && uorf(i->s.size) == 0)
		{
			//printf("2.a >= size: %d, is %s, and points to: %p\n", i->s.size, uorf(i->s.size)==1?"InUse":"Free", i->s.next_blck);
			if(i->s.size/sizeof(Header) == nsegments)	//if the size is exact
			{
				//printf("3.a = setting uorf of the block: %s, %d\n", uorf(i->s.size)==1?"InUse":"Free", i->s.size);
				i->s.size |= 0x01;	//set lsb to 1 (used)
				//printf("3.b = set uorf of the block    : %s, %d\n", uorf(i->s.size)==1?"InUse":"Free", i->s.size);
				return i+1;
			}
			else	//or if the size is larger than requested
			{
				//segment it from the end
				//printf("4.a before segmentation: \n");
				//printf("\tsize of i: %d\n", i->s.size);
				//printf("\tpoints to: %p\n", i->s.next_blck);

				i->s.size &= -2;	//set big block free
				printf("%d \n", i->s.size);
				i->s.size -= (nsegments+1)*sizeof(Header);
				printf("%d \n", i->s.size);

				Header *np = i+(i->s.size/16);
				np->s.size = nsegments*sizeof(Header);
				np->s.next_blck = i->s.next_blck;
				i->s.next_blck = np;

				//printf("4.b after segmentation: \n");
				//printf("\tsize of i:	%d\n", i->s.size);
				//printf("\tpoints to:	%p\n", i->s.next_blck);
				//printf("\tsize of h:	%d\n", np->s.size);
				//printf("\tpoints to:	%p\n", np->s.next_blck);
				//printf("\taddress  :	%p\n", np);

				//set the uorf
				//printf("3.a = setting uorf of the block: %s\n", uorf(np->s.size)==1?"InUse":"Free");
				//printf("3.ab = setting uorf of the block: %s\n", uorf(i->s.size)==1?"InUse":"Free");
				np->s.size |= 0x01;
				//printf("3.b = set uorf of the block    : %s\n", uorf(np->s.size)==1?"InUse":"Free");	
				//printf("3.bb = set uorf of the block    : %s\n", uorf(i->s.size)==1?"InUse":"Free");	

				//return i+1
				return np+1;
			}
		}
		else	//if block size is too small or the block is in use
		{	
			//printf("2.b < size: %d, is %s, and points to: %p\n", i->s.size, uorf(i->s.size)==1?"InUse":"Free", i->s.next_blck);
		}
		
		//Either made a full circle or just began
		if(i==fbp)
		{
			//Request memory either way, if requested < 1024, than request 1024
			if((i=memadd((nsegments*sizeof(Header)>=MALLOC_CHUNK_REQUEST)?nsegments:MALLOC_CHUNK_REQUEST))==NULL)
				return NULL;
		}
	}
}

//Used for freeing the allocated memory
void MFree(void *buffer)
{
	Header *hp = (Header *)buffer-1, *i;
	
	//printf("6.a free is called: %s\n", uorf(hp->s.size)==1?"InUse":"Free");
	hp->s.size &= ~(0x01);	//00000001 -> ~ znak je komplement -> 11111110 i kad sa tim and-ujes
	//printf("6.b free is exectd: %s\n", uorf(hp->s.size)==1?"InUse":"Free");
	
	for(i=fbp; ;i=i->s.next_blck)
		if(i->s.next_blck == hp)
			break;
	//printf("found i: %p, points to: i->s.next_blck: %p, hp: %p\n", i, i->s.next_blck, hp);
	//printf("i: %d, hp: %d\n", uorf(i->s.size), uorf(hp->s.size));
	//printf("i: %d, hp: %d\n", i->s.size, hp->s.size);
	//printf("i+size: %p == hp: %p\n", i+i->s.size, hp);

	if(uorf(i->s.size)==0 && i + i->s.size == hp)
        {
		printf("coalesing1");
                i->s.size += hp->s.size;        //combine the sizes
                i->s.next_blck = hp->s.next_blck;       //set next of i to be next of hp
        }
        //check if i->s.next_blck is free and adjacent to hp 
        if(uorf(i->s.size)==0 && hp + hp->s.size == i->s.next_blck)
        {
		printf("coalesing2");
                hp->s.size += i->s.next_blck->s.size;
                hp->s.next_blck = i->s.next_blck->s.next_blck;
        }
	

	return;
}

//Samo ispise info o allocated blokovima 
void PrintMmallocFreeList()
{

//-Logo
	printf("\n\n\t\t   .----.   \n");
	printf("\t\t   |@>_ |   \n");
	printf("\t\t __|____|__ \n");
	printf("\t\t|  ______--|\n");
	printf("\t\t`-/.::::.\\-'\n");
	printf("\t\t `--------' \n");
	printf("\t\t      Malloc implementation by -> mirkonikic\n");
	printf("\n");	
//-Iscrtaj listu gde h|i ili h|f predstavlja in_use_header or free_header i pored toga data data data...
	printf("\n\t[*]Verbose display of mmalloc heap memory:\n");

	Header *pi = lbp, *i;
	printf("\t[%c] : %dB : %p -> %p\n", uorf(pi->s.size)==1?'u':'f', pi->s.size, pi, pi->s.next_blck);
        if((pi=fbp)==NULL){printf("\t[*] list is currently empty\n\n");return;}
	
	for(i=pi->s.next_blck; i!=lbp; pi=i, i=i->s.next_blck)
	{
		//[h]:5123B:0xeff91627 -> 0xeffb7684
		printf("\t[%c] : %dB : %p -> %p\n", uorf(i->s.size)==1?'u':'f', i->s.size, i, i->s.next_blck);
		if(uorf(i->s.size)==1 && printf("\t\t %p: ", i)>0){
			for(int c = 0; c<(i->s.size)/16; c++){
				char *l = i+c;
				if(c!=0)	
					printf("\n\t\t %p: ", i+c);
				for(int k = 0; k<16; k+=4)
				{
					printf("0x%02hhx%02hhx%02hhx%02hhx ", *(l+c+k), *(l+c+k+1), *(l+c+k+2), *(l+c+k+3));
					//printf("%p %p %p %p \n", (l+c+k), (l+c+k+1), (l+c+k+2), (l+c+k+3));
				}
				printf("\t //");
				for(int k = 0; k<16; k+=4)
                                {
					printf("%c %c %c %c ", *(l+c+k), *(l+c+k+1), *(l+c+k+2), *(l+c+k+3));
                                }

			}
			printf("\n");
		}
		//for za prikaz podataka koji se nalaze unutar segmenta
	}
	printf("\n\n");
//-Prikazi memoriju kao u gdb-u ->h:0x41414140:f:0x12442123 0x1244223352 0x124241135 ->h:0x11111111:i:0x21313525523 0x14333525313 0x1315315132 ...
	
	return;
}

Header *memadd(size_t size)
{
	//printf("5.a memory is needed from kernel: %ldB\n", size*sizeof(Header));
	
	char *np;	//current pointer and next pointer after request for memory segment
	Header *hp, *i;	//header pointer

	np = sbrk(size*sizeof(Header));
	
	if(np == (char *)-1)
		return NULL;

	hp = (Header *)np;	//store address of pointer returned by sbrk to header pointer
	hp->s.size = size*sizeof(Header);	//set first block to size parameter

	//Chunk of code that was supposed to go to other function	

	//Conditions to stop:
        //between two blocks:   (hp>i && hp<i->s.next_blck) 
        //greater than the end: (i>i->s.next_blck && hp>i)
        //less than the base:   (i>i->s.next_blck && hp<i->s.next_blck)
	for(i = fbp; !(hp>i&&hp<i->s.next_blck)&&!(i>i->s.next_blck&&hp>i)&&!(i>i->s.next_blck&&hp<i->s.next_blck); i=i->s.next_blck)
		if(i->s.next_blck==i)
               		break; 

        //printf("5.b found a block: \n");
        //printf("\t i address:     %p\n", i);
	//printf("\t points to:     %p\n", i->s.next_blck);
        //printf("\t n address:     %p\n", np);
        //printf("\t fbp addrs:     %p\n", fbp);
        //printf("\t lbp addrs:     %p\n", lbp);
        //printf("\t hp addrss:     %p\n", hp);
	//printf("\t size in d:     %d\n", hp->s.size);

        //Check if i is only element of the list
	hp->s.next_blck = i->s.next_blck;
        i->s.next_blck = hp;
        fbp = i;
	
        //printf("5.c finished memadd: \n");
        //printf("\t i address:     %p\n", i);
	//printf("\t points to:     %p\n", i->s.next_blck);
        //printf("\t h address:     %p\n", hp);
	//printf("\t points to:     %p\n", hp->s.next_blck);
	//printf("\t size in d:     %d\n", hp->s.size);
        //printf("\t fbp addrs:     %p\n", fbp);
        //printf("\t lbp addrs:     %p\n", lbp);

        return fbp;
}

//Extract Least Significant Bit
//void prntaddr(void *addr){return;}	//prints data value from the address: 0x00101120 ...
int retsize(uint size){return size & -2;} 	//and sa 1111...1100, pa se izbegne posl bit
int uorf(uint size){return size & 0x00000001;}	//and sa 0000...0001, pa se extract posl bit
