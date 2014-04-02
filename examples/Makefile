# examples Makefile

CC=gcc
CC_INCLUDES=-I../build/deps/include -I../build/include
CC_LIBRARIES=-L../build/lib -L../build/deps/lib -lgsl -lgslcblas -lm -levolve

all:
	$(CC) knapsack/knapsack.c -o ../build/examples/knapsack $(CC_INCLUDES) $(CC_LIBRARIES)
clean:
	rm -f bin/knapsack
