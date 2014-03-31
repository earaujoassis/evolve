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

#ifndef __evolve_ga_ivf_h__
#define __evolve_ga_ivf_h__

/* Genetic Algorithm with IVF (In Vitro Fertilization) */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "repr_integer.h"
#include "stats.h"

#define IVFGA_DEFAULT_XOVER_POINTS      10
#define IVFGA_SUPER_PARENTS             population->size * 0.5

int_chrom_t **
ivfga_pselection (int_pop_t *population,
                  size_t num_parents)

void
ivfga_manipulation (int_chrom_t **parents,
                    size_t num_parents,
                    int_chrom_t *offspring,
                    double (*fitness_function) (int_chrom_t *),
                    int (*is_valid) (int_chrom_t *));

void
ivfga_transference (const int_pop_t *population,
                    int_chrom_t *manipulated);

stats_t *
int_ivfga (int_pop_t *population,
           size_t generations,
           double (*fitness_function) (int_chrom_t *),
           int (*is_valid) (int_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ga_ivf_h__

