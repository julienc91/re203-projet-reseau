SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
all: controler

trex.o: ../trex/trex.c
	$(CC) -o $@ -c $< $(CFLAGS)

controler: $(OBJ) trex.o
	$(CC) $^ -o $@  $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
