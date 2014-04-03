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

// Random-key Genetic Algorithm/Evolution Strategy

#ifndef __evolve_hybrid_real_ga_es_h__
#define __evolve_hybrid_real_ga_es_h__

#include <stdlib.h>

#include "evolve_repr_real.h"
#include "evolve_stats.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef TOURNAMENT_SIZE
#define TOURNAMENT_SIZE         5
#endif
#ifndef REGENERATED_PERCENTAGE
#define REGENERATED_PERCENTAGE  0.20
#endif

void
evolve_random_generated_individuals (evolve_real_pop_t *population,
                                     double population_percentage,
                                     int (*is_valid) (evolve_real_chrom_t *));

evolve_stats_t *
evolve_rkgaes (evolve_real_pop_t *population,
               size_t generations,
               double (*fitness_function) (evolve_real_chrom_t *),
               int (*is_valid) (evolve_real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_hybrid_real_ga_es_h__
