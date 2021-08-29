CC = gcc
EXECUTABLES = test1 test2 test3 test4

SRC = src
INCLUDE = include
BINARY = bin

LIBDIR = 
LIBS = 
CFLAGS = -g -Wall

all: $(SRC)/mmalloc.c $(INCLUDE)/mmalloc.h
	$(CC) $(CFLAGS) -c $(SRC)/mmalloc.c -o mmalloc
	chmod +x mmalloc
#all: $(EXECUTABLES)

clean: 
	rm -f core mmalloc

clean_all:
	rm -f core $(EXECUTABLES) mmalloc

mmalloc.o: mmalloc.c mmalloc.h
	$(CC) $(CFLAGS) -c $(SRC)/mmalloc.c -o mmalloc

mmalloc_nocompact.o: mmalloc.c mmalloc.h
	$(CC) $(CFLAGS) -c -DNO_COALESCE -o mmalloc_nocompact.o mmalloc.c

test1: test1.c mmalloc.o my_malloc.h
	$(CC) $(CFLAGS) -o test1 test1.c mmalloc.o $(LIBS)

test2: test2.c mmalloc.o my_malloc.h
	$(CC) $(CFLAGS) -o test2 test2.c mmalloc.o $(LIBS)

test3: test3.c mmalloc_nocompact.o my_malloc.h
	$(CC) $(CFLAGS) -o test3 test3.c mmalloc_nocompact.o $(LIBS)

test4: test4.c mmalloc.o mmalloc.h
	$(CC) $(CFLAGS) -o test4 test4.c mmalloc.o $(LIBS)
