SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(OBJ) trex.o

trex.o: ../trex/trex.c
	$(CC) -o $@ -c $< $(CFLAGS)

common: $(OBJ) trex.o
	$(CC) -lpthread $^ -o $@  $(LDFLAGS)

%.o: %.c
	$(CC) -std=c99 -o $@ -c $< $(CFLAGS)
