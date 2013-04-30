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


$(BINARY): main.cpp router.cpp event.cpp prompt.cpp exec.cpp prompt_actions.cpp display.cpp rout_table.cpp $(COMMONOBJ)
	@echo -n " Compiling" $@ "..."
	$(CC) $(CFLAGS) $^ -o $(BINARY) $(LFLAGS)
	@echo " Done."

clean:
	$(RM) *.o $(BINARY)
