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

#ifndef __evolve_stats_h__
#define __evolve_stats_h__

#include "evolve.h"
#include "evolve_repr_real.h"
#include "evolve_repr_integer.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
  double max_fitness;
  double ave_fitness;
  double min_fitness;
  short int first_time;
  size_t time;
  size_t evaluations_counted;
  size_t population_size;
} evolve_stats_t;

evolve_stats_t *
evolve_init_stats ();

void
evolve_del_stats (evolve_stats_t *);

void
evolve_reset_real_pop_stats (const evolve_real_pop_t *,
                             evolve_stats_t *);

void
evolve_reset_int_pop_stats (const evolve_int_pop_t *,
                            evolve_stats_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_stats_h__
