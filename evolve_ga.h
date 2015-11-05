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

// Canonical Genetic Algorithm

#ifndef __evolve_ga_canonical_h__
#define __evolve_ga_canonical_h__

#include "evolve.h"
#include "evolve_repr_integer.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef evolve_ga_t
#define evolve_ga_t evolve_object_t
#endif
#ifndef evolve_del_ga
#define evolve_del_ga evolve_del_object
#endif

evolve_ga_t *
evolve_ga_init ();

evolve_int_chrom_t *
evolve_ga_breed (const evolve_object_t *);

void
evolve_ga_per_int_mutation (const evolve_object_t *);

void
evolve_ga_npoint_xover (const evolve_object_t *);

void
evolve_ga_onepoint_xover (const evolve_object_t *);

void
evolve_ga_uniform_xover (const evolve_object_t *);

evolve_int_chrom_t **
evolve_ga_roulette_wheel_selection (const evolve_object_t *);

evolve_int_chrom_t **
evolve_ga_tournament_selection (const evolve_object_t *);

void
evolve_ga_elitist_policy_replacement (const evolve_object_t *);

void
evolve_ga_generational_replacement (const evolve_object_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ga_canonical_h__
