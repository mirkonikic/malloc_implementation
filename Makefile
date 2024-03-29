CC = gcc
EXECUTABLES = test1 test2 test3 test4

SRC = src
INCLUDE = include
OBJECT = obj
BINARY = bin

SRCFILES = $(wildcard src/*.c)
INCLUDEFILES = $(wildcard include/*.h)

LIBDIR = 
LIBS = $(INCLUDE)/mmalloc.h 
CFLAGS = -g -Wall

all: $(EXECUTABLES)

clean:
	rm -f core $(BINARY)/$(EXECUTABLES) a.out

mmalloc.o: $(SRC)/mmalloc.c $(INCLUDE)/mmalloc.h
	$(CC) $(CFLAGS) -c $(SRC)/mmalloc.c -o $(OBJECT)/mmalloc.o

mmalloc_nocompact.o: $(SRC)/mmalloc.c $(INCLUDE)/mmalloc.h
	$(CC) $(CFLAGS) -c -DNO_COALESCE -o $(OBJECT)/mmalloc_nocompact.o $(SRC)/mmalloc.c

test1: $(SRC)/test1.c mmalloc.o
	$(CC) $(CFLAGS) -o $(BINARY)/test1 $(SRC)/test1.c $(OBJECT)/mmalloc.o $(LIBS)

test2: $(SRC)/test2.c mmalloc.o
	$(CC) $(CFLAGS) -o $(BINARY)/test2 $(SRC)/test2.c $(OBJECT)/mmalloc.o $(LIBS)

test3: $(SRC)/test3.c mmalloc_nocompact.o
	$(CC) $(CFLAGS) -o $(BINARY)/test3 $(SRC)/test3.c $(OBJECT)/mmalloc_nocompact.o $(LIBS)

test4: $(SRC)/test4.c mmalloc.o
	$(CC) $(CFLAGS) -o $(BINARY)/test4 $(SRC)/test4.c $(OBJECT)/mmalloc.o $(LIBS)
