#include <stdio.h>
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

/*
 *	Header struct|16 consists of block_size and next_block pointer
 *	sbrk(MAX_MALLOC_SIZE) vrati pointer na novo alocirani segment u address space-u processa
 *	- Ako ne nadjem odgovarajuci sta radim?
 *		- Trazim jos memorije, samo treba da se iskontrolise da se doda na listu
 *	- Kako znam kad prekidam program?
 *		- Za svaki ishod mora da ima return iz petlje
 */

Header *lbp;	//List Base Pointer
Header *fbp = NULL;	//Free Block Pointer
Header *pi;	//Pointer to the previous of *i
//*pi is used to link previous block with next with regards to *i
//When that happens, fbp pointer is pointing to next block of *i

//InitMmalloc se koristi za inicijalizaciju/Kad je prazna lista sa blokovima
void InitMmalloc()
{
	printf("	MMALLOC		\n");
	printf("		by mirko\n");
	printf("		        \n");
	printf("sizeof(int): %ld\n", sizeof(int));
	printf("sizeof(Header): %ld\n", sizeof(Header));
	return;
}

//Mmalloc se koristi za alokaciju kad je poznata lista i ima u sebi blokove
void *Mmalloc(size_t size)
{
	Header *i;
	uint nsegments;	//Size of headers, since one block is the size of header
	
	//Sekvencijalna petlja koja prodje po blokovima i pregleda koji je slobodan blok
	nsegments = (size+sizeof(Header)-1)/sizeof(Header) + 1;
	printf("number of segments needed: %d\n", nsegments);

	//PROVERI DAL POSTOJI LISTA
	//AKO NE, PITAJ ZA MEMADD
	//	AKO NE USPE RETURN NULL
	//	ELSE CONTINUE
	
	//POSTAVI I NA PI->NEXT_BLCK
	//I kreni da analiziras blokove
	//Uporedjujes velicinu
	//
	//Sada postoje dva izlaza, ili nadje >= blok i podeli ga ili ga vrati
	//Ili vrti u krug dok se ne vrati na pocetnu velicinu
	//Ako se vrati da je i==blp, onda trazi memadd i ako ne uspe return NULL
	//Ako uspe nastavlja for loop jer sad ima dovoljno memorije
	for(i = pi->next_blck ; ; pi = i, i = i->next_blck)
	{
		//check the size of block i is pointing to
	}

	//Na pocetku treba da vidim dal ima liste uopste, odnosno na sta fbp pokazuje
	//	ako pokazuje na NULL onda nema liste...
	//	Potrebno je pozvati memadd onda
	//Ako memadd fail-uje onda return NULL;
	//Else zapocceti for petlju		//da bi bila beskonacna
	//For petlja pocinje sa p=pi->next_blck; ; pi=p, p=p->next_blck)
	//U petlji se proverava da li je trenutni blok in use ili free
	//Ako je free uporedi se da li ima dovoljnu velicinu
	//Ako nema ide u sledeci loop
	//Za problem sa vrtenjem u krug, mogu da namestim blp break loop pointer
	//	on se podesi na poziciju pi na pocetku i ako p==blp onda trazi memadd

	//Kako gledamo da li je blok inuse ili free
	//Po poslednjem bitu, objasnjeno je u header fajlu to vec
	//Ali otprilike ovako if(p->size&1 == 0b00000001) //blok is in use
	
	return NULL;
}

//MFree se isto koristi ako je vec poznata lista i pokusava da oslobodi blok na adresi
void MFree(void *buffer)
{
	printf("Free\n");
	return;
}

//Samo ispise info o allocated blokovima 
void PrintMmallocFreeList()
{
	printf("yessir\n");
	return;
}

void *memadd(size_t size)
{
	char  *addr;
	
	printf("Uspela alokacija? o.o\n");

	return addr;
}

//Extract Least Significant Bit
int retsize(uint size){return size & -2;} 	//and sa 1111...1100, pa se izbegne posl bit
int uorf(uint size){return size & 0x00000001;}	//and sa 0000...0001, pa se extract posl bit
