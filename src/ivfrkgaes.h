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

#ifndef __evolve_ga_es_ivf_h__
#define __evolve_ga_es_ivf_h__

/* Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization) */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define IVFGA_SUPER_PARENTS             population->size * 0.25
#define IVFGA_CUTOFFS_UP_BOUND          0.3
#define IVFGA_CUTOFFS_BOTTOM_BOUND      0.2

real_chrom_t **
ivfrkgaes_pselection (real_pop_t *population,
                      size_t num_parents);

size_t
ivfrkgaes_number_points (size_t chromosome_size);

size_t *
ivfrkgaes_xover_points (size_t chromosome_bound);

void
ivfrkgaes_manipulation (real_chrom_t **parents,
                        size_t num_parents,
                        real_chrom_t *offspring,
                        double (*fitness_function) (real_chrom_t *),
                        int (*is_valid) (real_chrom_t *));

void
ivfrkgaes_transference (const real_pop_t *population,
                        real_chrom_t *manipulated);

stats_t *
ivfrkgaes (real_pop_t *population,
           size_t generations,
           double (*fitness_function) (real_chrom_t *),
           int (*is_valid) (real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ga_es_ivf_h__
