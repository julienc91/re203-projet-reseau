CC=@gcc
RM=@rm -f
CFLAGS = -g -std=c99
LDFLAGS = -lpthread

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

.PHONY:	clean

all: $(OBJ) trex.o

trex.o: ../trex/trex.c
	$(CC) -o $@ -c $< $(CFLAGS)

common: $(OBJ) trex.o
	$(CC) -g -lpthread $^ -o $@  $(LDFLAGS)

%.o: %.c
	@echo -n " Compiling" $@ "..."
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)
	@echo " Done."

clean:
	$(RM) $(OBJ)
	@echo "Cleaned."
