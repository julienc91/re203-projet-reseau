CC=@gcc
CFLAGS=-Wall -Wextra -g -Wno-unused-parameter
LFLAGS=-lpthread

BINARY=router
COMMON_PATH  = ../common/
COMMON_FILES = prompt.o messages.o net.o trex.o

COMMONOBJ = $(addprefix $(COMMON_PATH), $(COMMON_FILES))


all: $(BINARY)

$(BINARY): main.c $(COMMONOBJ) 
	@echo -n " Compiling " $@ "..."
	$(CC) $(CFLAGS) $^ -o $(BINARY) $(LFLAGS)
	@echo " Done."
