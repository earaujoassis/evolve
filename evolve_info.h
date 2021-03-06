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

#ifndef __evolve_info_h__
#define __evolve_info_h__

#include "evolve.h"
#include "evolve_repr_real.h"
#include "evolve_repr_integer.h"
#include "evolve_stats.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void
evolve_print_algo_info (const char *,
                        const char *,
                        size_t,
                        size_t,
                        unsigned long int);
void
evolve_print_common_info (const evolve_stats_t *,
                          const evolve_stats_t *);

void
evolve_print_real_pop (const evolve_real_pop_t *,
                       const evolve_stats_t *);

void
evolve_print_int_pop (const evolve_int_pop_t *,
                      const evolve_stats_t *);

void
evolve_print_real_chrom (const evolve_real_chrom_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_info_h__
