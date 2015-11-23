SRCDIR = src
BINDIR = bin
TESTDIR = test
#FLAGS = -Wall -Wextra
FLAGS = -O3

.PHONY: test

all: $(BINDIR)/computePatch $(BINDIR)/applyPatch

$(BINDIR)/computePatch: $(SRCDIR)/main.cpp $(SRCDIR)/file.cpp $(SRCDIR)/file.hpp $(SRCDIR)/solver.cpp $(SRCDIR)/solver.hpp
	g++ $(FLAGS) $(SRCDIR)/main.cpp $(SRCDIR)/file.cpp $(SRCDIR)/solver.cpp -o $(BINDIR)/computePatch

$(BINDIR)/applyPatch: $(SRCDIR)/applyPatch.c
	gcc $(FLAGS) $(SRCDIR)/applyPatch.c -o $(BINDIR)/applyPatch

clean:
	rm $(BINDIR)/*

test: all
	cd $(TESTDIR) ; ../$(BINDIR)/computePatch source target > patch
	cd $(TESTDIR) ; ../$(BINDIR)/applyPatch patch source > target2
	cd $(TESTDIR) ; diff target target2
