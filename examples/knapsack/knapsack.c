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
#include "../src/ivfga.h"
#include "utils/fitness.h"
#include "utils/parser.h"

int
main (int argc, char **argv)
{
  int instances, counter;
  knapsack_t *problem;
  int_pop_t *population;
  stats_t *stats;

  while(scanf("%d\n", &instances) != EOF)
    for(counter = 0; counter < instances; counter++)
      {
        problem = init_instance (stdin);
        if (problem == NULL)
          return 0;

        set_problem (problem);
        population = init_int_pop (POP_SIZE);
        set_rng (SEED);
        random_int_pop (population, problem->items, &check);
        stats = int_ivfga (population, MAX_GEN, &fitness, &check);
        tear_rng ();
        print_common_info (stats, NULL);
        //print_int_pop (population, stats);
        //print_algo_info ("GA", "Multidimensional 0-1 Knapsack Problem", MAX_GEN, POP_SIZE, SEED);
        del_stats (stats);
        del_int_pop_indiv (population);
        del_int_pop (population);

        del_knapsack (problem);
      }
}

