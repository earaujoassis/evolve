// Copyright 2013-2015 Ewerton Assis
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

#ifndef __evolve_settings_h__
#define __evolve_settings_h__

#include "evolve.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef evolve_settings_t
#define evolve_settings_t evolve_object_t
#endif
#ifndef evolve_del_settings
#define evolve_del_settings evolve_del_object
#endif

evolve_settings_t *
evolve_settings_init ();

// It should set the check_chrom attr
// It should set the replacement_policy attr
//      (evolve_ga_elitist_policy_replacement, evolve_ga_generational_replacement)
// It should set the selection_strategy attr
//      (evolve_ga_tournament_selection, evolve_ga_roulette_wheel_selection)
// It should set the population attr
// It should set the generations attr
// It should set the fitness attr
// It should set the breed attr
//      (ivfga: evolve_ga_breed, ga: evolve_ga_breed, ev: evolve_ev_breed)
// It should set the recombination_strategy (Integer Chromosomes Recombination Strategies)
//      (evolve_ga_uniform_xover, evolve_ga_onepoint_xover, evolve_ga_npoint_xover - ivfga & ga)
// It should set the mutation_strategy
//      (evolve_ga_per_int_mutation)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ioc_settings_h__
