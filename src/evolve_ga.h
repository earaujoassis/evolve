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

#ifndef __evolve_ga_canonical_h__
#define __evolve_ga_canonical_h__

/* Canonical Genetic Algorithm */

#include <stdlib.h>

#include "evolve_repr_integer.h"
#include "evolve_stats.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef XOVER_RATE
#define XOVER_RATE         70.0
#endif
#ifndef MUTATION_RATE
#define MUTATION_RATE      0.20
#endif

void
evolve_int_uniform_xover (const evolve_int_chrom_t *parent_one,
                          const evolve_int_chrom_t *parent_two,
                          evolve_int_chrom_t *offspring,
                          double parameterized);

void
evolve_int_onepoint_xover (const evolve_int_chrom_t *parent_one,
                           const evolve_int_chrom_t *parent_two,
                           evolve_int_chrom_t *offspring);

void
evolve_int_npoint_xover (const evolve_int_chrom_t *parent_one,
                         const evolve_int_chrom_t *parent_two,
                         evolve_int_chrom_t *offspring,
                         size_t num_points);

void
evolve_per_int_mutation (evolve_int_chrom_t *chrom,
                         double mutation_chance);

evolve_int_chrom_t *
evolve_int_breed (const evolve_int_chrom_t *parent_one,
                  const evolve_int_chrom_t *parent_two,
                  size_t birthdate,
                  int (*is_valid) (evolve_int_chrom_t *));

evolve_int_chrom_t **
evolve_int_tournament_selection (const evolve_int_pop_t *population,
                                 size_t tournament_size,
                                 size_t total_selection);

evolve_int_chrom_t **
evolve_roulette_wheel_selection (const evolve_int_pop_t *population,
                                 size_t total_selection);

void
evolve_generational_replacement (evolve_int_pop_t *population,
                                 evolve_int_chrom_t **offspring);

void
evolve_elitist_policy_replacement (evolve_int_pop_t *population,
                                   evolve_int_chrom_t **offspring);

evolve_stats_t *
evolve_int_ga (evolve_int_pop_t *population,
               size_t generations,
               double (*fitness_function) (evolve_int_chrom_t *),
               int (*is_valid) (evolve_int_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ga_canonical_h__

