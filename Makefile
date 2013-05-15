export CFLAGS= -g -std=c99
export CPPFLAGS= -g
export LDFLAGS=-lpthread -lgraph -lcdt
export CC=@gcc
export CP=@g++
export RM=@rm -f
export CD=-@cd
export SILENT_MAKE=$(MAKE) -s

SRCA=$(wildcard common/*.c)
SRCB=$(wildcard controller/*.c)
SRCC=$(wildcard router/*.cpp)
OBJ=$(SRC:.c=.o)

.PHONY: clean common controller router doc rapport run

all:
	@$(SILENT_MAKE) common
	@$(SILENT_MAKE) controller
	@$(SILENT_MAKE) router


common: $(SRCA)
	@echo "common/"
	$(CD) common && $(MAKE)
	@echo ""

controller: $(SRCB)
	@echo "controller/"
	$(CD) controller && $(MAKE)
	@echo ""

router: $(SRCC)
	@echo "router/"
	$(CD) router && $(MAKE)
	@echo ""

doc:	doc/doxygen.conf
	doxygen doc/doxygen.conf

rapport: 
	$(CD) rapport && pdflatex rapport.tex && pdflatex rapport.tex
	$(CD) rapport && rm -r *.aux *.log *.out

run:	all
	./run.sh

clean:
	@echo "common/"
	$(CD) common && $(SILENT_MAKE)	clean
	@echo ""
	@echo "router/"
	$(CD) router && $(SILENT_MAKE) clean
	@echo ""
	@echo "controller/"
	$(CD) controller && $(SILENT_MAKE) clean
	@echo ""