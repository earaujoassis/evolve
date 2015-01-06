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

// EV: according to DE JONG's implementation

#ifndef __evolve_ev_h__
#define __evolve_ev_h__

#include "evolve_repr_real.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

evolve_real_chrom_t *
evolve_real_breed_ev (const evolve_real_chrom_t *parent,
                      int birthdate,
                      double step_size,
                      void (*mutation_function) (evolve_real_chrom_t *, double),
                      int (*is_valid) (evolve_real_chrom_t *));

void
evolve_real_ev (evolve_real_pop_t *population,
                long generations,
                double step_size,
                void (*mutation_function) (evolve_real_chrom_t *, double),
                double (*fitness_function) (evolve_real_chrom_t *),
                int (*is_valid) (evolve_real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ev_h__
