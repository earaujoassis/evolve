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

#include "utils/settings.h"
#include <evolve/paradigms/hybrid/rkgaes.h>
#include "utils/fitness.h"
#include "utils/parser.h"

int
main (int argc, char **argv)
{
  salesman_t *problem;
  real_pop_t *population;
  stats_t *stats;

  problem = init_instance (stdin);
  if (problem == NULL)
    return;
  set_problem (problem);
  population = init_real_pop (POP_SIZE);
  set_rng (SEED);
  random_real_pop (population, problem->dimension, 0, \
    problem->dimension, &mock_check);
  stats = rkgaes (population, MAX_GEN, &fitness, &mock_check);
  tear_rng ();
  print_common_info (stats, NULL);
  del_stats (stats);
  del_real_pop_indiv (population);
  del_real_pop (population);
  del_salesman (problem);

  return 0;
}

