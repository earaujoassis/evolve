.PHONY: build clean

BUILD_PATH=$(realpath ..)/build

build:
	(cd $(realpath .)/proto && autoreconf -i && ./configure --disable-shared --prefix=$(BUILD_PATH) && make && make install)
	(cd $(realpath .)/gsl && autoreconf -i -f && ./configure --disable-shared --prefix=$(BUILD_PATH) && make && make install)

clean:
	$(MAKE) -C gsl clean
