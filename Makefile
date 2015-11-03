# Top-level Makefile

.PHONY: dependencies build examples tests default clean all

dependencies:
	mkdir -p build
	mkdir -p build/deps
	$(MAKE) -C deps

build:
	mkdir -p build/obj
	mkdir -p build/lib
	mkdir -p build/include
	mkdir -p build/include/evolve
	$(MAKE) -C core

examples:
	mkdir -p build/examples
	$(MAKE) -C examples

tests:
	mkdir -p tests/bin
	$(MAKE) -C tests

all: dependencies build

default: all

clean:
	rm -rf build
	$(MAKE) -C deps clean
