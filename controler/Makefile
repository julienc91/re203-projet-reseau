CFLAGS= -g -std=c99
LDFLAGS=-lpthread -lgraph -lcdt
CC=@gcc
RM=@rm -f

BINARY = controller

COMMON_PATH  = ../common/
COMMON_FILES = prompt.o messages.o net.o trex.o util.o config.o client.o

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
COMMONOBJ = $(addprefix $(COMMON_PATH), $(COMMON_FILES))

.PHONY: clean

all: $(BINARY)

$(BINARY): $(OBJ) $(COMMONOBJ)
	@echo -n " Compiling" $@ "..."
	$(CC) $^ -o $@  $(LDFLAGS)
	@echo " Done."


%.o: %.c
	@echo -n " Compiling" $@  "..."
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo  " Done."

clean:
	$(RM) $(OBJ)
	@echo "Cleaned."
