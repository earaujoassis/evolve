# deps Makefile

include ../Makefile.am

BUILDPATH=$(realpath ..)/build
DEPPATH=$(BUILDPATH)/deps

.PHONY: all default clean

all:
	(cd $(realpath .)/gsl && autoreconf -i && ./configure --disable-shared --prefix=$(DEPPATH) && make && make install)
	(cd $(realpath .)/jansson && autoreconf -i && ./configure --disable-shared --prefix=$(DEPPATH) && make && make install)

default: all

clean:
	$(MAKE) -C gsl clean
	$(MAKE) -C jansson clean
