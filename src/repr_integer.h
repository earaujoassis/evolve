// Copyright 2011-2014 Ewerton Assis
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __evolve_repr_integer_h__
#define __evolve_repr_integer_h__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>
#include <stdio.h>

typedef struct {
  unsigned short int *vector;
  size_t size;
  size_t birthdate;
  double fitness;
} int_chrom_t;

typedef struct {
  int_chrom_t **individuals;
  size_t size;
  size_t generation;
  size_t birthcounter;
} int_pop_t;

int_chrom_t *
init_int_chrom (size_t size,
                size_t birthdate);

void
del_int_chrom (int_chrom_t *chrom);

int_chrom_t *
copy_int_chrom (const int_chrom_t *chrom,
                size_t birthdate);

void
apply_int_fitness (int_chrom_t *chrom,
                   double (*fitness_function) (int_chrom_t *));

int_pop_t *
init_int_pop (size_t size);

void
del_int_pop (int_pop_t *pop);

void
del_int_pop_indiv (int_pop_t *pop);

void
del_int_indiv (int_chrom_t **individuals,
               size_t size);

void
random_int_pop (int_pop_t *population,
                size_t chrom_size,
                int (*is_valid) (int_chrom_t *));

void
apply_int_pop_fitness (const int_pop_t *pop,
                       double (*fitness_function) (int_chrom_t *));

int_chrom_t *
best_int_chrom (const int_pop_t *pop);

int_chrom_t *
worst_int_chrom (const int_pop_t *pop);

size_t
worst_int_chrom_index (const int_pop_t *pop);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_repr_integer_h__
