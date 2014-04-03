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

// Integer Chromosomes Selection Strategies

#ifndef __evolve_int_sel_h__
#define __evolve_int_sel_h__

#include "evolve_repr_integer.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

evolve_int_chrom_t **
evolve_int_tournament_selection (const evolve_int_pop_t *population,
                                 size_t tournament_size,
                                 size_t total_selection);

evolve_int_chrom_t **
evolve_int_roulette_wheel_selection (const evolve_int_pop_t *population,
                                     size_t total_selection);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_int_sel_h__
