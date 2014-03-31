# Top-level Makefile

.PHONY:
	default

default:
	mkdir -p build
	mkdir -p build/deps
	(cd deps && make)
#	(cd examples && make)

clean:
	rm -rf build
	(cd deps && make clean)
