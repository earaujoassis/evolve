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

#include "evolve_breed.h"
#include "evolve_int_sel.h"
#include "evolve_int_rep.h"
#include "evolve_ga.h"

evolve_stats_t *
evolve_int_ga (evolve_int_pop_t *population,
               size_t generations,
               double (*fitness_function) (evolve_int_chrom_t *),
               int (*is_valid) (evolve_int_chrom_t *))
{
  size_t i;
  evolve_stats_t *stats = evolve_init_stats ();
  evolve_apply_int_pop_fitness (population, fitness_function);
  evolve_reset_int_pop_stats (population, stats);
  do
    {
      evolve_int_chrom_t **selection, **offspring;
      offspring = (evolve_int_chrom_t **) calloc (population->size, sizeof (evolve_int_chrom_t *));
#ifdef TOURNAMENT_SIZE
      selection = evolve_int_tournament_selection (population, TOURNAMENT_SIZE, population->size);
#else
      selection = evolve_int_roulette_wheel_selection (population, population->size);
#endif
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[2 * i] = evolve_int_breed (selection[2 * i], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 * i + 1] = evolve_int_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
#ifdef ELITIST_POLICY
      evolve_int_elitist_policy_replacement (population, offspring);
#else
      evolve_int_generational_replacement (population, offspring);
#endif
      free (offspring);
      evolve_apply_int_pop_fitness (population, fitness_function);
      evolve_reset_int_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}
