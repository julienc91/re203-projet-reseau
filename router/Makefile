CC=@g++
RM=@rm -f
CFLAGS=-Wall -Wextra -g -Wno-unused-parameter
LFLAGS=-lpthread

BINARY=router
COMMON_PATH  = ../common/
COMMON_FILES = prompt.o messages.o net.o config.o trex.o util.o

COMMONOBJ = $(addprefix $(COMMON_PATH), $(COMMON_FILES))

.PHONY:	clean

all: $(BINARY)

<<<<<<< HEAD
$(BINARY): main.cpp event.cpp exec.cpp rout_table.cpp $(COMMONOBJ)
=======
$(BINARY): main.cpp event.cpp exec.cpp net_actions.cpp display.cpp  $(COMMONOBJ)
>>>>>>> e07e109d4138247650bd1674e092cd47931951c3
	@echo -n " Compiling" $@ "..."
	$(CC) $(CFLAGS) $^ -o $(BINARY) $(LFLAGS)
	@echo " Done."

clean:
	$(RM) *.o $(BINARY)
