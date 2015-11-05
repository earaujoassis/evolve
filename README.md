# Evolve ![Travis-CI status](https://travis-ci.org/earaujoassis/evolve.svg?branch=master "Travis-CI status")

> A library for Evolutionary Computation algorithms

Evolve is a C library for metaheuristics algorithms. They are: genetic algorithm, In Vitro Fertilization
genetic algorithm (IVF-GA), ev (a very simple metaheuristic introduced in *Evolutionary Computation*, by
Kenneth De Jong), and some other implementation stubs.

## Build & Install

You must install Autotools before attempting to build & install it.

```sh
$ autoreconf -i
$ ./configure
$ make
$ make install
```

## Running the examples

```sh
$ autoreconf -i
$ mkdir -p build
$ CPPFLAGS=-I$PWD/build/include LDFLAGS=-L$PWD/lib ./configure --prefix=$PWD/build --disable-shared
$ make thirdparty
$ make
$ make install
$ make examples
$ examples/bin/2d-parabola
$ examples/bin/m-01-knapsack examples/m-01-knapsack-instance.txt
```

## Testing

Install Valgrind prior to running any further command. Also, make sure to build the library following
previous steps for running the examples.

```sh
$ make tests
```

## Paradigms and Metaheuristics implemented by include file

* EV (evolve/evolve_ev.h)
* Canonical Genetic Algorithm (evolve/evolve_ga.h)
* Genetic Algorithm with IVF (In Vitro Fertilization) (evolve/evolve_ivfga.h)
* Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization) (evolve/evolve_rkivfgaes.h)
* Random-key Genetic Algorithm/Evolution Strategy (evolve/evolve_rkgaes.h)

## Dependencies (available on this package)

 * [Proto](https://github.com/earaujoassis/proto)
 * [GSL (GNU Scientific Library)](http://www.gnu.org/software/gsl/)

## License

Please refer to the included *LICENSE file* for terms of use. Some examples used to showcase this project
are based upon instances available on scientific literature; thus those instances should be disregarded of
this *LICENSE*; they have their own copyright and license for distribution – the same applies to projects
located inside the `thirdparty` folder.

Apache License, Version 2.0. Copyright 2011-2015 &copy; Ewerton Assis.
