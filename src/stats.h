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

#ifndef __evolve_stats_h__
#define __evolve_stats_h__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "repr_real.h"

typedef struct {
  double max_fitness;
  double ave_fitness;
  double min_fitness;
  short int first_time;
  size_t time;
  size_t evaluations_counted;
  size_t population_size;
} stats_t;

stats_t *
init_stats ();

void
del_stats (stats_t *stats);

void
reset_real_pop_stats (const real_pop_t *pop,
                      stats_t *stats);

void
reset_int_pop_stats (const int_pop_t *pop,
                     stats_t *stats);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_stats_h__
