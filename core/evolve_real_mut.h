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

// Floating-point/Real Mutation Strategies

#ifndef __evolve_real_mut_h__
#define __evolve_real_mut_h__

#include "evolve_repr_real.h"
#include "evolve_permutation.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef MUTATION_RATE
#define MUTATION_RATE      0.150
#endif

void
evolve_real_delta_mutation (evolve_real_chrom_t *chrom,
                            double step_size);

void
evolve_real_gaussian_mutation (evolve_real_chrom_t *chrom,
                               double step_size);

void
evolve_real_permutation_mutation (evolve_real_chrom_t *offspring);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_real_mut_h__
