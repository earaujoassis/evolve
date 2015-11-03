# deps Makefile

include ../Makefile.am

BUILDPATH=$(realpath ..)/build
DEPPATH=$(BUILDPATH)/deps

.PHONY: all default clean

all:
	(cd $(realpath .)/proto && autoreconf -i && ./configure --disable-shared --prefix=$(DEPPATH) && make && make install)
	(cd $(realpath .)/gsl && autoreconf -i && ./configure --disable-shared --prefix=$(DEPPATH) && make && make install)

default: all

clean:
	$(MAKE) -C gsl clean
