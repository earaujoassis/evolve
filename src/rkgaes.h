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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define TOURNAMENT_SIZE         5
#define NPOINTS_XOVER           4
#define HEAD_TOSSING            70.0
#define REGENERATED_PERCENTAGE  0.20

void
uniform_xover (const real_chrom_t *parent_one,
               const real_chrom_t *parent_two,
               real_chrom_t *offspring,
               double parameterized);

void
onepoint_xover (const real_chrom_t *parent_one,
                const real_chrom_t *parent_two,
                real_chrom_t *offspring);

void
npoint_xover (const real_chrom_t *parent_one,
              const real_chrom_t *parent_two,
              real_chrom_t *offspring,
              size_t num_points,
              size_t *selected_points);

void
permutation_mutation (real_chrom_t *offspring);

void
random_generated_individuals (real_pop_t *population,
                              double population_percentage,
                              int (*is_valid) (real_chrom_t *));

real_chrom_t **
linear_ranking (const real_pop_t *population,
                size_t total_selection);

real_chrom_t **
tournament_selection (const real_pop_t *population,
                      size_t tournament_size,
                      size_t total_selection);

real_chrom_t **
fitness_proportional (const real_pop_t *population,
                      size_t total_selection);

real_chrom_t *
real_breed (const real_chrom_t *parent_one,
            const real_chrom_t *parent_two,
            size_t birthdate,
            int (*is_valid) (real_chrom_t *));

void
elitist_policy_replacement (real_pop_t *population,
                            real_chrom_t **offspring);

stats_t *
rkgaes (real_pop_t *population,
        size_t generations,
        double (*fitness_function) (real_chrom_t *),
        int (*is_valid) (real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_hybrid_real_ga_es_h__
