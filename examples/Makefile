# examples Makefile

CC=gcc
CC_INCLUDES=-I../build/deps/include -I../build/include
CC_LIBRARIES=-L../build/deps/lib -L../build/lib -lgsl -lgslcblas -lm

all:
	$(CC) knapsack/knapsack.c -o bin/knapsack $(CINCLUDES) $(CLIBRARIES) $(CFILES)
clean:
	rm -f bin/knapsack
