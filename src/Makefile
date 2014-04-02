# src Makefile

-include ../Makefile.am

CC=gcc
RELATIVE=../
C_FILES=$(wildcard *.c)
C_HEADERS=$(wildcard *.h)
OBJ_FILES=$(addprefix $(RELATIVE)build/obj/,$(notdir $(C_FILES:.c=.o)))
INCLUDE_FILES=$(addprefix $(RELATIVE)build/include/,$(notdir $(C_HEADERS:.h=.h)))
CC_FLAGS=-c -O -fPIC -DPIC
CC_INCLUDES=-I$(RELATIVE)build/deps/include
CC_LIBRARIES=-L$(RELATIVE)build/deps/lib -L$(RELATIVE)build/lib -lgsl -lgslcblas -lm

.PHONY: default

$(RELATIVE)build/obj/%.o: %.c
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) -o $@ $<

$(RELATIVE)build/obj/%.h: %.h
	cp $@ $<

default: $(OBJ_FILES)
	$(CC) -shared -Wl,-soname,$(RELATIVE)build/lib/levolve.so.$(VERSION.MAJOR) -o $(RELATIVE)build/lib/levolve.so.$(VERSION) $(OBJ_FILES) $(CC_LIBRARIES)
	(cp $(RELATIVE)build/lib/levolve.so.$(VERSION) $(RELATIVE)build/lib/levolve.so.$(VERSION.MAJOR))
