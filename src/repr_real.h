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

#ifndef __evolve_repr_real_h__
#define __evolve_repr_real_h__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
  double *vector;
  size_t size;
  size_t birthdate;
  double fitness;
} real_chrom_t;

typedef struct {
  real_chrom_t **individuals;
  size_t size;
  size_t generation;
  size_t birthcounter;
} real_pop_t;

real_chrom_t *
init_real_chrom (size_t size,
                 size_t birthdate);

void
del_real_chrom (real_chrom_t *chrom);

real_chrom_t *
copy_real_chrom (const real_chrom_t *chrom,
                 size_t birthdate);

void
apply_real_fitness (real_chrom_t *chrom,
                    double (*fitness_function) (real_chrom_t *));

real_pop_t *
init_real_pop (size_t size);

void
del_real_pop (real_pop_t *pop);

void
del_real_pop_indiv (real_pop_t *pop);

void
del_real_indiv (real_chrom_t **individuals,
                size_t size);

void
random_real_pop (real_pop_t *population,
                 int chrom_size,
                 double a,
                 double b,
                 int (*is_valid) (real_chrom_t *));

void
apply_real_pop_fitness (const real_pop_t *pop,
                        double (*fitness_function) (real_chrom_t *));

real_chrom_t *
best_real_chrom (const real_pop_t *pop);

real_chrom_t *
worst_real_chrom (const real_pop_t *pop);

size_t
worst_real_chrom_index (const real_pop_t *pop);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_repr_real_h__
