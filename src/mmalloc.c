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

static Header base;
static Header *lbp = &base;	//List Base Pointer
static Header *fbp = NULL;	//Free Block Pointer
//Header *pi;	//Pointer to the previous of *i, i moved to Mmalloc method
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
	Header *i, *pi;
	uint nsegments;	//Size of headers, since one block is the size of header
	
	//Sekvencijalna petlja koja prodje po blokovima i pregleda koji je slobodan blok
	nsegments = (size+sizeof(Header)-1)/sizeof(Header) + 1;
	printf("number of segments needed: %d\n", nsegments);

//After every mmalloc ill set fbp to pi, so that a reference to the last block visited is stored
//On beginning of the next mmalloc call, ill set pi to fbp so that i can continue from there and not start over
//Than ill check if fbp is == NULL, if it is, than the list is empty
//If it is not than the for loop starts

	//Ovde inicijalizujes listu
	//Lista se inicijalizuje kad pokazuje na sebe na pocetku
	//Pa polako dodas jedan po jedan clan
	if((pi=fbp)==NULL)
	{
		printf("pi je fbp sada\n");
		//ILI dodaj jos memorije jer na inicijalizaciji fbp ne pokazuje ninasta
		//Postavi da pokazuje na pocetak liste ili na sebe
		//Napravi prvi header, Podesi da sledeci od lbp bude lbp
		//Podesi Velicinu lbp na 0, sto kaze da je free ali ne postoji
		lbp->s.next_blck=pi=fbp=lbp;	//Set values for base header of the list
		lbp->s.size=0;	//Set values for base header of the list
	}

//In for loop, there are few possible events:
//1. found a chunk > than needed
	//i segment it and send i+1 address
//2. found a chunk = to what i need
	//i set the 1 least significant bit, so that it says used
	//and return i+1 address
//3. found a chunk < than what i need
	//continue, thats not what i need chief

//Also Not found a free chunk for a whole loop is a possibility, but thats not a one cycle option, but a whole loop
//If that happens i will check if i == fbp, than a whole circle has been made
			    //this will be infinite loop and exits with returns for each possible outcome
	//Popravi da prvo proveri dal je inuse ili free pa tek onda size
	//Olaksam procesoru broj instrukcija
	for(i = pi->s.next_blck ; ; pi = i, i = i->s.next_blck)
	{
		if(i->s.size >= nsegments)
		{
			printf("Nasao sam blok velicine >= %d: %d, %s, %p\n", nsegments, i->s.size, uorf(i->s.size)==1?"InUse":"Free", i->s.next_blck);
			//Zelim da vidim samo dal radi na pocetku
			//Ostatak koda se odnosi na dodeljivanje memorije korisniku
			//Prepravljanje headera
			//I promene InUse ili Free Bita
		}
		else
		{	
			printf("Nasao sam blok velicine < %d: %d, %s, %p\n", nsegments, i->s.size, uorf(i->s.size)==1?"InUse":"Free", i->s.next_blck);
		}
		
		
		//Na samom pocetku ce ovo biti true jer pi->next_blck pokazuje na sebe
		//Jer jos ne postoji lista heap alociranih blokova
		if(i==fbp)
		{
			//Ceo Loop je prosao
			printf("Presao sam ceo loop, nisam nasao odg blok..\n");
			//Trazim od kernela 5*zeljeni blok da ne bih pri svakom malloc pozivu morao da trazim od kernela memoriju, vec da mogu sl put da segmentiram
			//postavljam i na fbp koji je vracen iz zahteva za memadd, u sl ciklusu ce biti segmentiran i popravljen header
			//samo sam morao u free() da se pobrinem da se doda chunk na listu
			if((i=memadd((nsegments*sizeof(Header)>=MALLOC_CHUNK_REQUEST)?nsegments*sizeof(Header):MALLOC_CHUNK_REQUEST))==NULL)
				return NULL;
			return NULL;
		}
		//Ako nije nasao u ovom ciklusu odgovarajucu memoriju mozda je nadje u sledecem pa ne radim nista
	}
}

//MFree se isto koristi ako je vec poznata lista i pokusava da oslobodi blok na adresi
void MFree(void *buffer)
{
	Header *hp = (Header *)buffer-1, *i;
	
	//printf info about allocation/free-ing
	printf("Freepointer: %p\n", buffer);
	printf("Header:      %p\n", buffer-1);
	printf("\tHeader size: %d\n", hp->s.size);
        printf("Address lbp: %p\n", lbp);
       	printf("Address fbp: %p\n", fbp);
	
	//Prvo proveris za errors i da li je in use ovaj block
	//i nadjes odgovarajuci blok za allocation
	//zapocnes sa i=fbp i onda ides jedan po jedan dok ne nadjes odg
	//ako je i->s.next_blck == lbp, onda dodas samo na kraj
	//trazim da stane ako je hp adresa > od i a manja od i->s.next_blck
	//ili ako je sledeci pointer lbp sto znaci da je manja od base pointera ili veca od kraja liste
	//znaci ako je hp<lbp ili i->s.next_blck == lbp && hp>i
	
	//stop if following conditions are met
	//between two blocks:	(hp>i && hp<i->s.next_blck) 
	//greater than the end:	(i>i->s.next_blck && hp>i)
	//less than the base:	(i>i->s.next_blck && hp<i->s.next_blck)
	for(i = fbp; !(hp>i&&hp<i->s.next_blck)||!(i>i->s.next_blck&&hp>i)||!(i>i->s.next_blck&&hp<i->s.next_blck); i=i->s.next_blck)
		if(i->s.next_blck==i)
			break;
	
	printf("!!!CONDITION IS MET\n");
	printf("address of current pointer: 	%p\n", i);
	printf("address of next pointer: 	%p\n", i->s.next_blck);
	printf("address of fbp pointer: 	%p\n", fbp);
	printf("address of lbp pointer: 	%p\n", lbp);
	printf("address of header pointer: 	%p\n", hp);
	
	//now im not sure which condition made for loop break but,
	//in either way i should implement next few lines
	//hp->s.next_blck = i->s.next_blck
	//i->s.next_blck = hp
	
	//check if adjacent blocks are also free
	//	check if i is free
	//		set i size to theirs combined and set i to point to hp->s.next_blck
	//	check if next block is free //hp+hp->s.size==i->s.next_blck
	//		set the size to be combined and set hp to point to next block
	//or if it points to itself, than dont
	//	set hp to point to i->s.next_blck
	//	set i to point to hp
	
	if(i->s.next_blck==i){
		hp->s.next_blck = i->s.next_blck;
		i->s.next_blck = hp;
		fbp = i;
		printf("sbrk-ed memory to address: %p, pointing from fbp: %p, and pointer addres is: %p\nand hp pointing to: %p\n", hp, fbp, fbp->s.next_blck, hp->s.next_blck);
		return;
	}
	//check if i pointer is free and adjacent to hp
	//after knowing its not pointing to itself
	if(uorf(i->s.size)==0 && i + i->s.size == hp)
	{
		i->s.size += hp->s.size;	//combine the sizes
		i->s.next_blck = hp->s.next_blck;	//set next of i to be next of hp
	}
	//check if i->s.next_blck is free and adjacent to hp 
	if(uorf(i->s.size)==0 && hp + hp->s.size == i->s.next_blck)
	{
		hp->s.size += i->s.next_blck->s.size;
		hp->s.next_blck = i->s.next_blck->s.next_blck;
	}			
	
	

	return;
}

//Samo ispise info o allocated blokovima 
void PrintMmallocFreeList()
{
	printf("yessir\n");
	//For petljom prelistaj sve blokove
	//Iscrtaj trenutnu memoriju kao u gdb mozda
	return;
}

Header *memadd(size_t size)
{
	//Zatrazi od kernela jos memorije
	//Pa free-uj (addr+1) pointer, da bi se dodao u fbp listu i napravio odg header
	printf("Zatrazeno je %ldB\n", size*sizeof(Header));
	
	char *cp, *np;	//current pointer and next pointer after request for memory segment
	Header *hp;	//header pointer

	cp = sbrk(0);
	np = sbrk(size*sizeof(Header));
	
	printf("current heap address: %p\n", cp);
	printf("after heap pointer:   %p\n", np);
	printf("free block pointer:   %p\n", np+1);
	if(np == (char *)-1)
		return NULL;

	//by calling sbrk, i get additional 'size_t size' bytes to use, on past program break
	//but the current break is moved down the memory by the 'size_t size' bytes
	
	//creating header at the beginning of the block
	hp = (Header *)np;	//store address of pointer returned by sbrk to header pointer
	hp->s.size = size;	//set first block to size parameter

	printf("hp: %p, %d\n", hp, hp->s.size);

	MFree((void *)(hp+1));	//send first address after hp to free, so that free() knows hp-1 is a header, which will shift down 16B
	printf("Returned to memadd :D\n");
	return fbp;
}

//Extract Least Significant Bit
//void prntaddr(void *addr){return;}
int retsize(uint size){return size & -2;} 	//and sa 1111...1100, pa se izbegne posl bit
int uorf(uint size){return size & 0x00000001;}	//and sa 0000...0001, pa se extract posl bit
