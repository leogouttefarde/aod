CC=gcc
CXX=g++
LATEXC=pdflatex
DOCC=doxygen

# If DEBUG not manually defined, define it here
ifndef DEBUG
	DEBUG=0
endif

CFLAGS= -Wall
CXXFLAGS= -Wall -Wextra -ansi -Werror

ifeq ($(DEBUG), 1)
	CFLAGS += -g
	# CXXFLAGS += -DPAPI -g
	CXXFLAGS += -DPAPI -O3
	LDFLAGS= -lpapi
else
	CFLAGS += -DNDEBUG -Wextra -O3
	CXXFLAGS += -O3
endif


REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport
TESTDIR=$(REFDIR)/test

LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
CSOURCE=$(wildcard $(SRCDIR)/applyPatch.c)
CPPSOURCES=$(wildcard $(SRCDIR)/*.cpp)
COMPUTEPATCH=$(SRCDIR)/computePatchOpt.cpp $(SRCDIR)/file.cpp $(SRCDIR)/solver.cpp
PDF=$(LATEXSOURCE:.tex=.pdf)


all: binary report doc 


$(BINDIR)/applyPatch: $(CSOURCE)
	$(CC) $(CFLAGS)  $^ -o $@

$(BINDIR)/computePatchOpt: $(COMPUTEPATCH)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(BINDIR)/patchCost: $(SRCDIR)/patchCost.cpp $(SRCDIR)/file.cpp $(SRCDIR)/file.hpp
	$(CXX) $(CXXFLAGS) $^ -o $@

%.pdf: $(LATEXSOURCE)
	cd $(REPORTDIR); $(LATEXC) $(LATEXSOURCE:$(REPORTDIR)/%=%); cd ..;

$(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE) $(CPPSOURCES)
	cd $(REFDIR); $(DOCC) $(SRCDIR:$(REFDIR)/%=%)/Doxyfile; cd ..;

binary: $(BINDIR)/applyPatch $(BINDIR)/computePatchOpt $(BINDIR)/patchCost

report: $(PDF) 

doc: $(DOCDIR)/index.html

clean:
	rm -f $(DOCDIR)/search/*
	if [ -d "$(DOCDIR)/search" ]; then	rmdir --ignore-fail-on-non-empty $(DOCDIR)/search;	fi
	rm -f $(DOCDIR)/* $(BINDIR)/* $(REPORTDIR)/*.{aux,log}

run: binary
	$(BINDIR)/computePatchOpt benchmark/benchmark_06/{source,target}

check:
	$(SRCDIR)/check.sh benchmark/benchmark_06/{source,target}


.PHONY: all doc binary report test check
