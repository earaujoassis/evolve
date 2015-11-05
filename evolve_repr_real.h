// Copyright 2011-2015 Ewerton Assis
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

#include "evolve.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
  double *vector;
  size_t size;
  size_t birthdate;
  double fitness;
} evolve_real_chrom_t;

typedef struct {
  evolve_real_chrom_t **individuals;
  size_t size;
  size_t generation;
  size_t birthcounter;
} evolve_real_pop_t;

evolve_real_chrom_t *
evolve_init_real_chrom (size_t,
                        size_t);

void
evolve_del_real_chrom (evolve_real_chrom_t *);

evolve_real_chrom_t *
evolve_copy_real_chrom (const evolve_real_chrom_t *,
                        size_t);

void
evolve_apply_real_fitness (evolve_real_chrom_t *,
                           double (*fitness_function) (evolve_real_chrom_t *));

evolve_real_pop_t *
evolve_init_real_pop (size_t);

void
evolve_del_real_pop (evolve_real_pop_t *);

void
evolve_del_real_pop_indiv (evolve_real_pop_t *);

void
evolve_del_real_indiv (evolve_real_chrom_t **,
                       size_t);

void
evolve_random_real_pop (evolve_real_pop_t *,
                        int,
                        double,
                        double,
                        int (*check_chrom) (evolve_real_chrom_t *));

void
evolve_apply_real_pop_fitness (const evolve_real_pop_t *,
                               double (*fitness_function) (evolve_real_chrom_t *));

evolve_real_chrom_t *
evolve_best_real_chrom (const evolve_real_pop_t *);

evolve_real_chrom_t *
evolve_worst_real_chrom (const evolve_real_pop_t *);

size_t
evolve_worst_real_chrom_index (const evolve_real_pop_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_repr_real_h__
