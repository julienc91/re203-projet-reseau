export CFLAGS= -g -std=c99
export LDFLAGS=-lpthread -lgraph -lcdt
export CC=gcc


SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
COMMONOBJ = prompt.o messages.o net.o
all: common controler



trex.o: ../trex/trex.c
	$(CC) -o $@ -c $< $(CFLAGS)

controler: $(OBJ)  $(COMMONOBJ) trex.o
	$(CC) $^ -o $@  $(LDFLAGS)

common:
	cp ../common/prompt.o ../common/messages.o ../common/net.o .

.PHONY: common

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
