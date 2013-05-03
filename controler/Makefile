CFLAGS= -g -std=c99
LDFLAGS=-lpthread -lgraph -lcdt
CC=@gcc
RM=@rm -f

COMMON_PATH  = ../common/
COMMON_FILES = prompt.o messages.o net.o trex.o util.o config.o

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
COMMONOBJ = $(addprefix $(COMMON_PATH), $(COMMON_FILES))


all:	controler


controler: $(OBJ) $(COMMONOBJ)
	$(CC) $^ -o $@  $(LDFLAGS)

.PHONY: clean

%.o: %.c
	@echo -n " Compiling" $@  "..."
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo  " Done."

clean:
	$(RM) $(OBJ)
	@echo "Cleaned."
