export CFLAGS= -g -std=c99
export LDFLAGS=-lpthread -lgraph -lcdt
export CC=@gcc
export RM=@rm -f

SRCA=$(wildcard common/*.c)
SRCB=$(wildcard controler/*.c)
SRCC=$(wildcard router/*.c)
OBJ=$(SRC:.c=.o)


all: common controler router

common: $(SRCA)
	@cd common && $(MAKE)

controler: $(SRCB)
	@cd controler && $(MAKE)

router: $(SRCC)
	@cd router && $(MAKE)


.PHONY: clean common controler router

clean:
	$(RM) common/*.o
	$(RM) controler/*.o
	$(RM) router/*.o
	@echo "Cleaned."
