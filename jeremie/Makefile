CC=gcc
CXX=g++
LATEXC=pdflatex
DOCC=doxygen
# CFLAGS=-g -Wall
CFLAGS= -Wall -Wextra -O3
# CXXFLAGS= -Wall -Wextra -pedantic -ansi -Weffc++ -g
CXXFLAGS= -Wall -Wextra -pedantic -ansi -Werror -O3

REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport

LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
CSOURCE=$(wildcard $(SRCDIR)/applyPatch.c)
CPPSOURCES=$(wildcard $(SRCDIR)/*.cpp)
COMPUTEPATCH=$(SRCDIR)/computePatch.cpp $(SRCDIR)/file.cpp $(SRCDIR)/solver.cpp
PDF=$(LATEXSOURCE:.tex=.pdf)


all: binary report doc 


$(BINDIR)/applyPatch: $(CSOURCE)
	$(CC) $(CFLAGS)  $^ -o $@

$(BINDIR)/computePatchOpt: $(COMPUTEPATCH)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BINDIR)/patchCost: $(SRCDIR)/patchCost.cpp $(SRCDIR)/file.cpp $(SRCDIR)/file.hpp
	$(CXX) $(CXXFLAGS) $^ -o $@

%.pdf: $(LATEXSOURCE)
	$(LATEXC) -output-directory $(REPORTDIR) $^ 

$(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE) 
	$(DOCC) $(SRCDIR)/Doxyfile

binary: $(BINDIR)/applyPatch $(BINDIR)/computePatchOpt $(BINDIR)/patchCost

report: $(PDF) 

doc: $(DOCDIR)/index.html

clean:
	rm -f $(DOCDIR)/search/*
	if [ -d "$(DOCDIR)/search" ]; then	rmdir --ignore-fail-on-non-empty $(DOCDIR)/search;	fi
	rm -f $(DOCDIR)/* $(BINDIR)/* $(REPORTDIR)/*.{aux,log} $(REPORTDIR)/rapport.pdf



test: all
	cd $(TESTDIR) ; ../$(BINDIR)/computePatch source target > patch
#	cd $(TESTDIR) ; valgrind --tool=cachegrind ../$(BINDIR)/computePatch source target > patch
#	cd $(TESTDIR) ; valgrind --leak-check=full ../$(BINDIR)/computePatch source target > patch
	cd $(TESTDIR) ; ../$(BINDIR)/applyPatch patch source > target2
	cd $(TESTDIR) ; diff target target2
	rm -f $(TESTDIR)/target2


.PHONY: all doc binary report test
