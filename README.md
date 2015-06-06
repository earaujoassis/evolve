# Evolve ![Travis-CI status](https://travis-ci.org/earaujoassis/evolve.svg?branch=master "Travis-CI status")

> A library for Evolutionary Computation algorithms

**WARNING**: CURRENTLY NOT READY FOR USE.

Evolve is a C library for metaheuristics algorithms. They are: genetic algorithm, In Vitro Fertilization
genetic algorithm (IVF-GA), ev (a very simple metaheuristic introduced in *Evolutionary Computation*, by
Kenneth De Jong), and some implementation stubs. The metaheuristics implemented are:

* Genetic algorithms (DE JONG, K. *Evolutionary Computation: a unified approach*. MIT Press, Cambridge,
2006.)

* EV, a simple evolutionary algorithm (DE JONG, K. *Evolutionary Computation: a unified approach*. MIT
Press, Cambridge, 2006.)

* IVF-GA, a hybrid genetic algorithm (CAMILO-JUNIOR, C. G.; YAMANAKA, K. *In vitro fertilization genetic
algorithm*. In: *Evolutionary Algorithms*, p. 57&ndash;68. InTech, 2011.)

* Random keys representation scheme (BEAN, J. C. *Genetic Algorithms and Random Keys for Sequencing and
Optimization*. *ORSA Journal on Computing*, 6:154&ndash;160, 1994.)

## Try it!

0. Clone this repo

   ```sh
   $ git clone git@github.com:earaujoassis/evolve.git
   ```

1. Install build dependencies

   ```sh
   $ sudo apt-get install build-essential texinfo
   ```

2. Build Evolve's dependencies and Evolve's library (all static libraries)

   ```sh
   $ make dependencies
   $ make build
   ```

3. Build and run an example for the Multidimensional 0-1 Knapsack Problem

   ```sh
   $ make examples
   $ build/examples/knapsack examples/knapsack/instances/mknapcb0.txt
   ```

## Paradigms and Metaheuristics implemented by include file

* EV (evolve_ev.h)
* Canonical Genetic Algorithm (evolve/evolve_ga.h)
* Genetic Algorithm with IVF (In Vitro Fertilization) (evolve/evolve_ivfga.h)
* Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization) (evolve/evolve_ivfrkgaes.h)
* Random-key Genetic Algorithm/Evolution Strategy (evolve/evolve_rkgaes.h)

## Dependencies (available on this package)

 * [GSL (GNU Scientific Library)](http://www.gnu.org/software/gsl/)
 * [Jansson](http://www.digip.org/jansson/)

## Working with the Web Server

    ```sh
    $ cd front
    $ virtualenv venv
    $ . venv/bin/activate
    $ pip install -r requirements.txt
    ```

## License

Please refer to the included *LICENSE file* for terms of use. Some examples used to showcase this project
are based upon instances available on scientific literature; thus those instances should be disregarded of
this *LICENSE*; they have their own copyright and license for distribution – the same applies to projects
located inside the `deps` folder.

Apache License, Version 2.0. Copyright 2011-2015 &copy; Ewerton Assis.
