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

#include <math.h>

#include <evolve/evolve_settings.h>
#include <evolve/evolve_ev.h>
#include <evolve/evolve_info.h>

#define SEED 987654
#define POP_SIZE 100

double
fitness_function (evolve_real_chrom_t *individual)
{
  // f(x) = 50 - pow (x, 2)
  return (double) - (50 - pow (individual->vector[0], 2));
}

int
check_chrom (evolve_real_chrom_t *individual)
{
  return individual->vector[0] < 100.00;
}

int
main (int argc, char **argv)
{
  evolve_real_pop_t *population;
  evolve_stats_t *stats;
  evolve_settings_t *settings;
  evolve_ev_t *solution;

  size_t generations = 1000;
  double mutation_step_size = 0.01;
  double mutation_rate = 0.15;

  evolve_set_rng (SEED);
  population = evolve_init_real_pop (POP_SIZE);
  evolve_random_real_pop (population, 1, 0, 100, &check_chrom);

  settings = evolve_settings_init ();
  settings->set_own_property (settings, "population", population);
  settings->set_own_property (settings, "generations", &generations);
  settings->set_own_property (settings, "fitness", &fitness_function);
  settings->set_own_property (settings, "breed", &evolve_ev_breed);
  settings->set_chain (settings, "strategies.check_chrom", &check_chrom);
  settings->set_chain (settings, "strategies.mutation_strategy", &evolve_ev_delta_mutation);
  settings->set_chain (settings, "strategies.mutation.step_size", &mutation_step_size);
  settings->set_chain (settings, "strategies.mutation.rate", &mutation_rate);

  solution = evolve_ev_init ();
  stats = (evolve_stats_t *) solution->execute_property (solution, "solution", settings);

  evolve_print_algo_info ("EV:", "f(x) = 50 - pow (x, 2)", generations, POP_SIZE, SEED);
  evolve_print_real_pop (population, stats);

  evolve_tear_rng ();
  evolve_del_settings (settings);
  evolve_del_ev (solution);
  evolve_del_stats (stats);
  evolve_del_real_pop_indiv (population);
  evolve_del_real_pop (population);
  return 0;
}
