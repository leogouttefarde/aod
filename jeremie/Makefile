SRCDIR = src
BINDIR = bin
TESTDIR = test
FLAGS = -Wall -Wextra -g
#FLAGS = -O3

.PHONY: test

all: clean $(BINDIR)/computePatch $(BINDIR)/applyPatch $(BINDIR)/patchCost

$(BINDIR)/computePatch: $(SRCDIR)/computePatch.cpp $(SRCDIR)/file.cpp $(SRCDIR)/file.hpp $(SRCDIR)/solver.cpp $(SRCDIR)/solver.hpp
	g++ $(FLAGS) $(SRCDIR)/computePatch.cpp $(SRCDIR)/file.cpp $(SRCDIR)/solver.cpp -o $(BINDIR)/computePatch

$(BINDIR)/patchCost: $(SRCDIR)/patchCost.cpp $(SRCDIR)/file.cpp $(SRCDIR)/file.hpp
	g++ $(FLAGS) $(SRCDIR)/patchCost.cpp $(SRCDIR)/file.cpp -o $(BINDIR)/patchCost

$(BINDIR)/applyPatch: $(SRCDIR)/applyPatch.c
	gcc $(SRCDIR)/applyPatch.c -o $(BINDIR)/applyPatch

clean:
	rm -f $(BINDIR)/*

test: all
#	cd $(TESTDIR) ; --tool=cachegrind ../$(BINDIR)/computePatch source target > patch
	cd $(TESTDIR) ; valgrind --tool=cachegrind ../$(BINDIR)/computePatch source target > patch
	cd $(TESTDIR) ; ../$(BINDIR)/applyPatch patch source > target2
	cd $(TESTDIR) ; diff target target2
	rm -f $(TESTDIR)/target2
