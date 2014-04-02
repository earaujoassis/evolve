# Top-level Makefile

.PHONY: dependencies build examples default clean

dependencies:
	mkdir -p build
	mkdir -p build/deps
	(cd deps && make)

build:
	mkdir -p build/obj
	mkdir -p build/lib
	mkdir -p build/include
	mkdir -p build/include/evolve
	(cd src && make)

examples:
	mkdir -p build/examples
	(cd examples && make)

default:
	dependencies
	build
	examples

clean:
	rm -rf build
	(cd deps && make clean)
