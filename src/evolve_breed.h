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

#ifndef __evolve_breed_h__
#define __evolve_breed_h__

#include <stdlib.h>

#include "evolve_repr_real.h"
#include "evolve_repr_integer.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef XOVER_RATE
#define XOVER_RATE         70.0
#endif
#ifndef MUTATION_RATE
#define MUTATION_RATE      0.20
#endif
#ifndef NPOINTS_XOVER
#define NPOINTS_XOVER           4
#endif
#ifndef HEAD_TOSSING
#define HEAD_TOSSING            70.0
#endif

evolve_int_chrom_t *
evolve_int_breed (const evolve_int_chrom_t *parent_one,
                  const evolve_int_chrom_t *parent_two,
                  size_t birthdate,
                  int (*is_valid) (evolve_int_chrom_t *));

evolve_real_chrom_t *
evolve_real_breed (const evolve_real_chrom_t *parent_one,
                   const evolve_real_chrom_t *parent_two,
                   size_t birthdate,
                   int (*is_valid) (evolve_real_chrom_t *));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_breed_h__
