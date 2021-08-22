#ifndef M_MALLOC_H
#define M_MALLOC_H

#define MAX_MALLOC_SIZE (1024*1024*16)

void InitMmalloc();
void *Mmalloc(int size);
void MFree(void *buffer);
void PrintMmallocFreeList();

#endif /* M_MALLOC_H */
