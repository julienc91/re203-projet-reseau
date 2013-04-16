SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
all: common

trex.o: ../trex/trex.c
	$(CC) -o $@ -c $< $(CFLAGS)

common: $(OBJ) trex.o
	$(CC) $^ -o $@  $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
