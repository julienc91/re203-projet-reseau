CC=gcc
CFLAGS=-Wall -Wextra -g -Wno-unused-parameter
LFLAGS=

BINARY=router

all: $(BINARY)


net.o:	../src/net.c ../src/net.h
	$(CC) $(CFLAGS) -o net.o -c $< $(LFLAGS)

$(BINARY): net.o main.c
	$(CC) $(CFLAGS) main.c $^ -o $(BINARY) $(LFLAGS)

