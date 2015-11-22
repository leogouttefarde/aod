
CC=gcc
CXX=g++
CFLAGS = -Wall -Wextra -std=c99 -g -O3
# CXXFLAGS = -Wall -Wextra -g -O3
CXXFLAGS = -Wall -Wextra -O3
# LDFLAGS = -lm #-s
LDFLAGS = -lm -s
EXEC=computePatchOpt


all: $(EXEC) applyPatch

$(EXEC): $(EXEC).cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

applyPatch: applyPatch.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c %.h
	$(CC) -o $@ -c  $< $(CFLAGS)

run: $(EXEC)
	./$(EXEC) tests/test_entree.txt tests/test_sortie.txt

clean:
	rm -f $(EXEC) applyPatch


