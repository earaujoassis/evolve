# Examples Makefile

CC=gcc
CINCLUDES=-I../build/deps/include -I../src
CLIBRARIES=-L../build/deps/lib -lgsl -lgslcblas -lm
CFILES=
all:
	$(CC) knapsack/knapsack.c -o bin/knapsack $(CINCLUDES) $(CLIBRARIES) $(CFILES)
clean:
	rm -f bin/knapsack

