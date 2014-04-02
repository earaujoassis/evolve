# deps Makefile

RELATIVE=../

.PHONY:
	default
	make-prerequisites
	make-gsl
	clean

make-gsl:
	(cd gsl-1.16 && ./configure --prefix=$(shell readlink -f $(RELATIVE))/build/deps && make && make install)

make-prerequisites:
	make-gsl

default:
	make-prerequisites

clean:
	(cd gsl-1.16 && make clean)
