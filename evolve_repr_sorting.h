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

#ifndef __evolve_sorting_h__
#define __evolve_sorting_h__

#include "evolve.h"
#include "evolve_repr_integer.h"
#include "evolve_repr_real.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void
evolve_qsort_int_chrom (evolve_int_chrom_t **,
                        long int,
                        long int);

void
evolve_qsort_real_chrom (evolve_real_chrom_t **,
                         long int,
                         long int);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_sorting_h__
