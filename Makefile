export CFLAGS= -g -std=c99
export CPPFLAGS= -g
export LDFLAGS=-lpthread -lgraph -lcdt
export CC=@gcc
export CP=@g++
export RM=@rm -f
export CD=-@cd
export SILENT_MAKE=@$(MAKE) -s

SRCA=$(wildcard common/*.c)
SRCB=$(wildcard controller/*.c)
SRCC=$(wildcard router/*.cpp)
OBJ=$(SRC:.c=.o)


all:
	$(SILENT_MAKE) common
	$(SILENT_MAKE) controller
	$(SILENT_MAKE) router


common: $(SRCA)
	@echo "common/"
	$(CD) common && $(MAKE)
	#@echo "Done."
	@echo ""

controller: $(SRCB)
	@echo "controller/"
	$(CD) controller && $(MAKE)
	#@echo "Done."
	@echo ""

router: $(SRCC)
	@echo "router/"
	$(CD) router && $(MAKE)
	#@echo "Done."
	@echo ""

.PHONY: clean common controller router

clean:
	$(RM) common/*.o
	$(RM) controller/*.o
	$(RM) router/*.o
	@echo "Cleaned."
