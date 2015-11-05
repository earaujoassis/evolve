.PHONY: build clean

ROOT=$(realpath ..)
BUILD_PATH=$(ROOT)/build
BIN_PATH=$(realpath .)/bin

CUSTOM_LIB=-L$(BUILD_PATH)/lib -levolve -lproto -lgsl -lgslcblas -lm
CUSTOM_INCLUDES=-I$(BUILD_PATH)/include
CUSTOM_FLAGS=-g

build:
	mkdir -p bin
	$(CC) $(CUSTOM_FLAGS) m-01-knapsack.c -o $(BIN_PATH)/m-01-knapsack $(CUSTOM_INCLUDES) $(CUSTOM_LIB)
	$(CC) $(CUSTOM_FLAGS) 2d-parabola.c -o $(BIN_PATH)/2d-parabola $(CUSTOM_INCLUDES) $(CUSTOM_LIB)

clean:
	rm -rf bin
	rm -f Makefile Makefile.in
	rm -f scripts/*.pyc
