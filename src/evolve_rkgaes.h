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

#ifndef __evolve_hybrid_real_ga_es_h__
#define __evolve_hybrid_real_ga_es_h__

/* Random-key Genetic Algorithm/Evolution Strategy */

#include <stdlib.h>

#include "evolve_repr_real.h"
#include "evolve_stats.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define TOURNAMENT_SIZE         5
#define NPOINTS_XOVER           4
#define HEAD_TOSSING            70.0
#define REGENERATED_PERCENTAGE  0.20

void
evolve_real_uniform_xover (const evolve_real_chrom_t *parent_one,
                           const evolve_real_chrom_t *parent_two,
                           evolve_real_chrom_t *offspring,
                           double parameterized);

void
evolve_real_onepoint_xover (const evolve_real_chrom_t *parent_one,
                            const evolve_real_chrom_t *parent_two,
                            evolve_real_chrom_t *offspring);

void
evolve_real_npoint_xover (const evolve_real_chrom_t *parent_one,
                          const evolve_real_chrom_t *parent_two,
                          evolve_real_chrom_t *offspring,
                          size_t num_points,
                          size_t *selected_points);

void
evolve_permutation_mutation (evolve_real_chrom_t *offspring);

void
evolve_random_generated_individuals (evolve_real_pop_t *population,
                                     double population_percentage,
                                     int (*is_valid) (evolve_real_chrom_t *));

evolve_real_chrom_t **
evolve_linear_ranking (const evolve_real_pop_t *population,
                       size_t total_selection);

evolve_real_chrom_t **
evolve_real_tournament_selection (const evolve_real_pop_t *population,
                                  size_t tournament_size,
                                  size_t total_selection);

evolve_real_chrom_t **
evolve_fitness_proportional (const evolve_real_pop_t *population,
                             size_t evolve_total_selection);

evolve_real_chrom_t *
evolve_real_breed (const evolve_real_chrom_t *parent_one,
                   const evolve_real_chrom_t *parent_two,
                   size_t birthdate,
                   int (*is_valid) (evolve_real_chrom_t *));

void
evolve_elitist_policy_replacement (evolve_real_pop_t *population,
                                   evolve_real_chrom_t **offspring);

evolve_stats_t *
evolve_rkgaes (evolve_real_pop_t *population,
               size_t generations,
               double (*fitness_function) (evolve_real_chrom_t *),
               int (*is_valid) (evolve_real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_hybrid_real_ga_es_h__
