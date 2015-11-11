CC=gcc
CXX=g++
LATEXC=pdflatex
DOCC=doxygen
CFLAGS=-g -Wall
CXXFLAGS=-g -Wall

REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport

LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
CSOURCE=$(wildcard $(SRCDIR)/applyPatch.c)
CPPSOURCES=$(wildcard $(SRCDIR)/*.cpp)
PDF=$(LATEXSOURCE:.tex=.pdf)


all: binary report doc 


$(BINDIR)/applyPatch: $(CSOURCE)
	$(CC) $(CFLAGS)  $^ -o $@

$(BINDIR)/computePatchOpt: $(CPPSOURCES)
	$(CXX) $(CXXFLAGS)  $^ -o $@

%.pdf: $(LATEXSOURCE)
	$(LATEXC) -output-directory $(REPORTDIR) $^ 

$(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE) 
	$(DOCC) $(SRCDIR)/Doxyfile

binary: $(BINDIR)/applyPatch $(BINDIR)/computePatchOpt

report: $(PDF) 

doc: $(DOCDIR)/index.html

clean:
	rm -f $(DOCDIR)/search/*
	if [ -d "$(DOCDIR)/search" ]; then	rmdir --ignore-fail-on-non-empty $(DOCDIR)/search;	fi
	rm -f $(DOCDIR)/* $(BINDIR)/* $(REPORTDIR)/*.{aux,log} $(REPORTDIR)/rapport.pdf


.PHONY: all doc binary report 
