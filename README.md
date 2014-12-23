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

## Paradigms and Metaheuristics implemented by include file

* EV (evolve_ev.h)
* Canonical Genetic Algorithm (evolve_ga.h)
* Genetic Algorithm with IVF (In Vitro Fertilization) (evolve_ivfga.h)
* Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization) (evolve_ivfrkgaes.h)
* Random-key Genetic Algorithm/Evolution Strategy (evolve_rkgaes.h)

## Dependencies (available on this package)

 * [GSL (GNU Scientific Library)](http://www.gnu.org/software/gsl/)
 * [Jansson](http://www.digip.org/jansson/)

## License

Please refer to the included *LICENSE file* for terms of use. Some examples used to showcase this project
are based upon instances available on scientific literature; thus those instances should be disregarded of
this *LICENSE*; they have their own copyright and license for distribution – the same applies to projects
located inside the `deps` folder.

Apache License, Version 2.0. Copyright 2011-2014 &copy; Ewerton Assis.
