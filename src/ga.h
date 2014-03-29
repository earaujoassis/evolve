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

#ifndef __evolve_ga_canonical_h__
#define __evolve_ga_canonical_h__

/* Canonical Genetic Algorithm */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef XOVER_RATE
#define XOVER_RATE         70.0
#endif
#ifndef MUTATION_RATE
#define MUTATION_RATE      0.20
#endif

void
uniform_xover (const int_chrom_t *parent_one,
               const int_chrom_t *parent_two,
               int_chrom_t *offspring,
               double parameterized);

void
onepoint_xover (const int_chrom_t *parent_one,
                const int_chrom_t *parent_two,
                int_chrom_t *offspring);

void
npoint_xover (const int_chrom_t *parent_one,
              const int_chrom_t *parent_two,
              int_chrom_t *offspring,
              size_t num_points);

void
per_int_mutation (int_chrom_t *chrom,
                  double mutation_chance);

int_chrom_t *
int_breed (const int_chrom_t *parent_one,
           const int_chrom_t *parent_two,
           size_t birthdate,
           int (*is_valid) (int_chrom_t *));

int_chrom_t **
tournament_selection (const int_pop_t *population,
                      size_t tournament_size,
                      size_t total_selection);

int_chrom_t **
roulette_wheel_selection (const int_pop_t *population,
                          size_t total_selection);

void
generational_replacement (int_pop_t *population,
                          int_chrom_t **offspring);

void
elitist_policy_replacement (int_pop_t *population,
                            int_chrom_t **offspring);

stats_t *
int_ga (int_pop_t *population,
        size_t generations,
        double (*fitness_function) (int_chrom_t *),
        int (*is_valid) (int_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ga_canonical_h__

