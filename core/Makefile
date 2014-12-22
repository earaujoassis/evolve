# core Makefile

include ../Makefile.am

BUILDPATH=$(realpath ..)/build
DEPPATH=$(BUILDPATH)/deps
LIBPATH=$(BUILDPATH)/lib
INCLUDEPATH=$(BUILDPATH)/include
OBJPATH=$(BUILDPATH)/obj

CC=gcc
AR=ar

CC_FLAGS=-c -O
CC_INCLUDES=-I$(DEPPATH)/include
CC_LIBRARIES=-L$(DEPPATH)/lib -L$(LIBPATH) -lgsl -lgslcblas -lm

AR_FLAGS=rcs

C_FILES=$(wildcard *.c)
C_HEADERS=$(wildcard *.h)
OBJ_FILES=$(addprefix $(OBJPATH)/,$(notdir $(C_FILES:.c=.o)))
INCLUDE_FILES=$(addprefix $(INCLUDEPATH)/evolve/,$(notdir $(C_HEADERS:.h=.h)))

.PHONY: all default

$(OBJPATH)/%.o: %.c
	$(CC) $(CC_FLAGS) $(CC_LIBRARIES) $(CC_INCLUDES) -o $@ -c $<

$(INCLUDEPATH)/evolve/%.h: %.h
	cp $< $@

all: $(OBJ_FILES) $(INCLUDE_FILES)
	$(AR) $(AR_FLAGS) $(LIBPATH)/libevolve.a $(OBJ_FILES)

default:
	all
