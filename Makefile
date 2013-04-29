export CFLAGS= -g -std=c99
export LDFLAGS=-lpthread -lgraph -lcdt
export CC=@gcc
export RM=@rm -f
export CD=-@cd
export SILENT_MAKE=@$(MAKE) -s

SRCA=$(wildcard common/*.c)
SRCB=$(wildcard controler/*.c)
SRCC=$(wildcard router/*.c)
OBJ=$(SRC:.c=.o)


all:
	$(SILENT_MAKE) common
	$(SILENT_MAKE) controler
	$(SILENT_MAKE) router


common: $(SRCA)
	@echo "common/"
	$(CD) common && $(MAKE)
	#@echo "Done."
	@echo ""

controler: $(SRCB)
	@echo "controller/"
	$(CD) controler && $(MAKE)
	#@echo "Done."
	@echo ""

router: $(SRCC)
	@echo "router/"
	$(CD) router && $(MAKE)
	#@echo "Done."
	@echo ""

.PHONY: clean common controler router

clean:
	$(RM) common/*.o
	$(RM) controler/*.o
	$(RM) router/*.o
	@echo "Cleaned."
