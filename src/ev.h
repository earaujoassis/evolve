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

#ifndef __evolve_ev_h__
#define __evolve_ev_h__

/* EV: according to DE JONG's implementation */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define DEFAULT_MUTATION_RATE      0.150

void
real_delta_mutation (real_chrom_t *chrom,
                     double step_size);

void
real_gaussian_mutation (real_chrom_t *chrom,
                        double step_size);

real_chrom_t *
real_breed (const real_chrom_t *parent,
            int birthdate,
            double step_size,
            void (*mutation_function) (real_chrom_t *, double),
            int (*is_valid) (real_chrom_t *));

void
real_ev (real_pop_t *population,
         long generations,
         double step_size,
         void (*mutation_function) (real_chrom_t *, double),
         double (*fitness_function) (real_chrom_t *),
         int (*is_valid) (real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ev_h__
