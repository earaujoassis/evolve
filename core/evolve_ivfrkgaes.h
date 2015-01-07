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

// Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization)

#ifndef __evolve_ga_es_ivf_h__
#define __evolve_ga_es_ivf_h__

#include <stdlib.h>

#include "evolve_repr_real.h"
#include "evolve_stats.h"
#include "evolve_defaults.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

evolve_real_chrom_t **
evolve_ivfrkgaes_pselection (evolve_real_pop_t *population,
                             size_t num_parents);

size_t
evolve_ivfrkgaes_number_points (size_t chromosome_size);

size_t *
evolve_ivfrkgaes_xover_points (size_t chromosome_bound);

void
evolve_ivfrkgaes_manipulation (evolve_real_chrom_t **parents,
                               size_t num_parents,
                               evolve_real_chrom_t *offspring,
                               double (*fitness_function) (evolve_real_chrom_t *),
                               int (*is_valid) (evolve_real_chrom_t *));

void
evolve_ivfrkgaes_transference (const evolve_real_pop_t *population,
                               evolve_real_chrom_t *manipulated);

evolve_stats_t *
evolve_ivfrkgaes (evolve_real_pop_t *population,
                  size_t generations,
                  double (*fitness_function) (evolve_real_chrom_t *),
                  int (*is_valid) (evolve_real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ga_es_ivf_h__
