.PHONY: build clean

ROOT=$(realpath ..)
BUILD_PATH=$(ROOT)/build
SUITES_PATH=$(realpath .)/suites
BIN_PATH=$(realpath .)/bin

CUSTOM_LIB=-L$(BUILD_PATH)/lib -levolve -lproto -lgsl -lgslcblas -lm
CUSTOM_INCLUDES=-I$(BUILD_PATH)/include
CUSTOM_FLAGS=-g

build:
	mkdir -p bin

clean:
	rm -rf bin
	rm -f Makefile Makefile.in
	rm -f scripts/*.pyc
