# tests Makefile

include ../Makefile.am

BUILDPATH=$(realpath ..)/build
DEPPATH=$(BUILDPATH)/deps
LIBPATH=$(BUILDPATH)/lib
INCLUDEPATH=$(BUILDPATH)/include
TESTSBINPATH=$(realpath .)/bin

CC=gcc
CC_INCLUDES=-I$(DEPPATH)/include -I$(INCLUDEPATH)
CC_LIBRARIES=-L$(DEPPATH)/lib -L$(LIBPATH) -levolve -lgsl -lgslcblas -lm

.PHONY: all default clean

all:
	$(CC) suites/test_datastructures.c -o $(TESTSBINPATH)/test_datastructures $(CC_INCLUDES) $(CC_LIBRARIES)

default: all

clean:
	rm -f $(TESTSBINPATH)/test_datastructures
