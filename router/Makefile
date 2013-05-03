CC=@g++
RM=@rm -f
CFLAGS=-Wall -Wextra -g -Wno-unused-parameter
LFLAGS=-lpthread

BINARY=router
COMMON_PATH  = ../common/
COMMON_FILES = messages.o net.o config.o trex.o util.o

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
COMMONOBJ = $(addprefix $(COMMON_PATH), $(COMMON_FILES))

.PHONY:	clean

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
	$(RM) *.o $(BINARY)
