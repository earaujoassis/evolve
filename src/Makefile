# src Makefile

-include ../Makefile.am

CC=gcc
RELATIVE=../
C_FILES=$(wildcard *.c)
C_HEADERS=$(wildcard *.h)
OBJ_FILES=$(addprefix $(RELATIVE)build/obj/,$(notdir $(C_FILES:.c=.o)))
INCLUDE_FILES=$(addprefix $(RELATIVE)build/include/evolve/,$(notdir $(C_HEADERS:.h=.h)))
CC_FLAGS=-c -O -fPIC -DPIC
CC_INCLUDES=-I$(RELATIVE)build/deps/include
CC_LIBRARIES=-L$(RELATIVE)build/deps/lib -L$(RELATIVE)build/lib -lgsl -lgslcblas -lm

.PHONY: default

$(RELATIVE)build/obj/%.o: %.c
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) -o $@ $<

$(RELATIVE)build/include/evolve/%.h: %.h
	cp $< $@

default: $(OBJ_FILES) $(INCLUDE_FILES)
	$(CC) -shared -Wl,-rpath='$ORIGIN' -o $(RELATIVE)build/lib/libevolve.so.$(VERSION) $(OBJ_FILES) $(CC_LIBRARIES)
	(cp $(RELATIVE)build/lib/libevolve.so.$(VERSION) $(RELATIVE)build/lib/libevolve.so.$(VERSION.MAJOR))
	(cp $(RELATIVE)build/lib/libevolve.so.$(VERSION) $(RELATIVE)build/lib/libevolve.so)
	ar rc $(RELATIVE)build/lib/libevolve.a $(OBJ_FILES)
