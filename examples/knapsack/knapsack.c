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

#include "utils/settings.h"

#include <evolve/evolve_ivfga.h>
#include <evolve/evolve_info.h>

#include "utils/fitness.h"
#include "utils/parser.h"

int
main (int argc, char **argv)
{
  int instances, counter;
  knapsack_t *problem;
  evolve_int_pop_t *population;
  evolve_stats_t *stats;

  while(scanf("%d\n", &instances) != EOF)
    for(counter = 0; counter < instances; counter++)
      {
        problem = init_instance (stdin);
        if (problem == NULL)
          return 0;

        set_problem (problem);
        population = evolve_init_int_pop (POP_SIZE);
        evolve_set_rng (SEED);
        evolve_random_int_pop (population, problem->items, &check);
        stats = evolve_int_ivfga (population, MAX_GEN, &fitness, &check);
        evolve_tear_rng ();
        evolve_print_common_info (stats, NULL);
        //evolve_print_int_pop (population, stats);
        //evolve_print_algo_info ("GA", "Multidimensional 0-1 Knapsack Problem", MAX_GEN, POP_SIZE, SEED);
        evolve_del_stats (stats);
        evolve_del_int_pop_indiv (population);
        evolve_del_int_pop (population);

        del_knapsack (problem);
      }
}

