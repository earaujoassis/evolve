# examples Makefile

RELATIVE=../
CC=gcc
CC_INCLUDES=-I$(RELATIVE)build/deps/include -I$(RELATIVE)build/include
CC_LIBRARIES=-L$(RELATIVE)build/lib -L$(RELATIVE)build/deps/lib -lgsl -lgslcblas -lm -levolve

all:
	$(shell export LD_LIBRARY_PATH=$(shell readlink -f $(RELATIVE))/build/lib:$(shell readlink -f $(RELATIVE))/build/deps/lib:$($$LD_LIBRARY_PATH))
	$(CC) knapsack/knapsack.c -o ../build/examples/knapsack $(CC_INCLUDES) $(CC_LIBRARIES)
clean:
	rm -f bin/knapsack
