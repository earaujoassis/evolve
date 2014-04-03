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

#ifndef __evolve_permutation_h__
#define __evolve_permutation_h__

#include "evolve_repr_real.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef PROB_EXCHANGE
#define PROB_EXCHANGE           35.0 // %
#endif
#ifndef PROB_INSERTION
#define PROB_INSERTION          35.0 // %
#endif
#ifndef PROB_INVERSION
#define PROB_INVERSION          10.0 // %
#endif
#ifndef PROB_SHIFTING
#define PROB_SHIFTING           20.0 // %
#endif

void
evolve_inversion_operator (evolve_real_chrom_t *chrom);

void
evolve_insertion_operator (evolve_real_chrom_t *chrom);

void
evolve_exchange_operator (evolve_real_chrom_t *chrom); /* Swapping */

void
evolve_shifting_operator (evolve_real_chrom_t *chrom); /* Long distance movement */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_permutation_h__
