CC=@g++
RM=@rm -f
CFLAGS=-Wall -Wextra -g -Wno-unused-parameter
LFLAGS=-lpthread

BINARY=router
COMMON_PATH  = ../common/
COMMON_FILES = prompt.o messages.o net.o trex.o

COMMONOBJ = $(addprefix $(COMMON_PATH), $(COMMON_FILES))

.PHONY:	clean

all: $(BINARY)

$(BINARY): main.cpp event.cpp exec.cpp rout_table.cpp $(COMMONOBJ)
	@echo -n " Compiling" $@ "..."
	$(CC) $(CFLAGS) $^ -o $(BINARY) $(LFLAGS)
	@echo " Done."

clean:
	$(RM) *.o $(BINARY)
