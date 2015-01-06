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

// Floating-point/Real Recombination Strategies

#ifndef __evolve_real_rec_h__
#define __evolve_real_rec_h__

#include "evolve_repr_real.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void
evolve_real_uniform_xover (const evolve_real_chrom_t *parent_one,
                           const evolve_real_chrom_t *parent_two,
                           evolve_real_chrom_t *offspring,
                           double parameterized);

void
evolve_real_onepoint_xover (const evolve_real_chrom_t *parent_one,
                            const evolve_real_chrom_t *parent_two,
                            evolve_real_chrom_t *offspring);

void
evolve_real_npoint_xover (const evolve_real_chrom_t *parent_one,
                          const evolve_real_chrom_t *parent_two,
                          evolve_real_chrom_t *offspring,
                          size_t num_points,
                          size_t *selected_points);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_real_rec_h__
