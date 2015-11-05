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

// Genetic Algorithm/Evolution Strategy

#ifndef __evolve_hybrid_ga_es_h__
#define __evolve_hybrid_ga_es_h__

#include "evolve.h"
#include "evolve_repr_real.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

evolve_real_chrom_t *
evolve_gaes_breed (const evolve_object_t *);

void
evolve_gaes_permutation_mutation (const evolve_object_t *);

void
evolve_gaes_random_generated_individuals (const evolve_object_t *);

void
evolve_gaes_npoint_xover (const evolve_object_t *);

void
evolve_gaes_onepoint_xover (const evolve_object_t *);

void
evolve_gaes_uniform_xover (const evolve_object_t *);

evolve_real_chrom_t **
evolve_gaes_fitness_proportional (const evolve_object_t *);

evolve_real_chrom_t **
evolve_gaes_tournament_selection (const evolve_object_t *);

evolve_real_chrom_t **
evolve_gaes_linear_ranking (const evolve_object_t *);

void
evolve_gaes_elitist_policy_replacement (const evolve_object_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_hybrid_ga_es_h__
