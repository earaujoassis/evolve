# examples Makefile

include ../Makefile.am

BUILDPATH=$(realpath ..)/build
DEPPATH=$(BUILDPATH)/deps
LIBPATH=$(BUILDPATH)/lib
INCLUDEPATH=$(BUILDPATH)/include
EXAMPLESPATH=$(BUILDPATH)/examples

CC=gcc
CC_INCLUDES=-I$(DEPPATH)/include -I$(INCLUDEPATH)
CC_LIBRARIES=-L$(DEPPATH)/lib -L$(LIBPATH) -lgsl -lgslcblas -lm -levolve

.PHONY: all default clean

all:
	$(CC) knapsack/knapsack.c -o $(EXAMPLESPATH)/knapsack $(CC_INCLUDES) $(CC_LIBRARIES)

default: all

clean:
	rm -f $(EXAMPLESPATH)/knapsack
