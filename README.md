# Evolve ![Travis-CI status](https://travis-ci.org/earaujoassis/evolve.svg?branch=master "Travis-CI status")

> A library for Evolutionary Computation algorithms

Evolve is a C library for metaheuristics algorithms. They are: genetic algorithm, In Vitro Fertilization
genetic algorithm (IVF-GA), ev (a very simple metaheuristic introduced in *Evolutionary Computation*, by
Kenneth De Jong), and some other implementation stubs.

## Setup & Running

Clone this project and load its submodules

```sh
$ git clone git@github.com:earaujoassis/evolve.git
$ git submodule update --init --recursive
```

Install build dependencies: on Linux/Ubuntu systems it would be something like `$ sudo apt-get install build-essential texinfo`;
on other systems, please make sure to have GCC, Texinfo and The Autotools installed.

Build Evolve's dependencies and Evolve's library (all static libraries)

```sh
$ make dependencies
$ make build
```

3. Build and run an example for the Multidimensional 0-1 Knapsack Problem

```sh
$ make examples
$ build/examples/knapsack examples/knapsack/instances/mknapcb0.txt
```

## Testing

Install Valgrind prior to running any further command. Also, make sure to build the library following
previous steps.

```sh
$ make tests
```

## Paradigms and Metaheuristics implemented by include file

* EV (evolve/evolve_ev.h)
* Canonical Genetic Algorithm (evolve/evolve_ga.h)
* Genetic Algorithm with IVF (In Vitro Fertilization) (evolve/evolve_ivfga.h)
* Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization) (evolve/evolve_ivfrkgaes.h)
* Random-key Genetic Algorithm/Evolution Strategy (evolve/evolve_rkgaes.h)

## Dependencies (available on this package)

 * [GSL (GNU Scientific Library)](http://www.gnu.org/software/gsl/)
 * [Jansson](http://www.digip.org/jansson/)

## License

Please refer to the included *LICENSE file* for terms of use. Some examples used to showcase this project
are based upon instances available on scientific literature; thus those instances should be disregarded of
this *LICENSE*; they have their own copyright and license for distribution – the same applies to projects
located inside the `deps` folder.

Apache License, Version 2.0. Copyright 2011-2015 &copy; Ewerton Assis.
