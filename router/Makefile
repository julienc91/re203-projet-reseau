CC=@g++
RM=@rm -f
CFLAGS=-Wall -Wextra -g -Wno-unused-parameter
LFLAGS=-lpthread

BINARY=router
COMMON_PATH  = ../common/
COMMON_FILES = messages.o net.o config.o trex.o util.o client.o

SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
COMMONOBJ = $(addprefix $(COMMON_PATH), $(COMMON_FILES))

.PHONY:	clean

all: $(BINARY)


$(BINARY): main.cpp router.cpp event.cpp prompt.cpp exec.cpp prompt_actions.cpp display.cpp entry.cpp sock_actions.cpp $(COMMONOBJ)

	@echo -n " Compiling" $@ "..."
	$(CC) $^ -o $(BINARY)  $(LDFLAGS)
	@echo " Done."

%.o: %.c
	@echo -n " Compiling" $@  "..."
	$(CC) -o $@ -c $< $(CFLAGS)
	@echo  " Done."

clean:
	$(RM) *.o $(BINARY)
