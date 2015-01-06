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

#include "evolve_rng.h"
#include "evolve_breed.h"
#include "evolve_repr_sorting.h"
#include "evolve_real_sel.h"
#include "evolve_real_rep.h"
#include "evolve_randomkey.h"
#include "evolve_o_rkgaes.h"
#include "evolve_rkgaes.h"

void
evolve_random_generated_individuals (evolve_real_pop_t *population,
                                     double population_percentage,
                                     int (*is_valid) (evolve_real_chrom_t *))
{
  size_t i, j;
  size_t amount, dimension;
  size_t chromosome_size = population->individuals[0]->size;
  amount = population->size * population_percentage;
  dimension = population->size / 2;
  evolve_qsort_real_chrom (population->individuals, 0, population->size - 1);
  for (i = population->size - amount; i < population->size; i++)
    {
      evolve_real_chrom_t *individual = population->individuals[i];
      evolve_del_real_chrom (individual);
      population->birthcounter++;
      individual = evolve_init_real_chrom (chromosome_size, population->birthcounter);
      do
        for (j = 0; j < chromosome_size; j++)
          individual->vector[j] = gsl_ran_flat (rng, 0, dimension);
      while (!(*is_valid) (individual));
      population->individuals[i] = individual;
    }
}

evolve_stats_t *
evolve_rkgaes (evolve_real_pop_t *population,
               size_t generations,
               double (*fitness_function) (evolve_real_chrom_t *),
               int (*is_valid) (evolve_real_chrom_t *))
{
  size_t i;
  evolve_stats_t *stats = evolve_init_stats ();
  evolve_apply_real_pop_fitness (population, fitness_function);
  evolve_reset_real_pop_stats (population, stats);
  do
    {
      evolve_real_chrom_t **selection, **offspring;
      offspring = (evolve_real_chrom_t **) calloc (population->size, sizeof (evolve_real_chrom_t *));
      if (global_options->selection == proportional)
        selection = evolve_real_fitness_proportional (population, population->size);
      else if (global_options->selection == tournament)
        selection = evolve_real_tournament_selection (population, TOURNAMENT_SIZE, population->size);
      else if (global_options->selection == ranking)
        selection = evolve_real_linear_ranking (population, population->size);
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[i * 2] = evolve_real_breed (selection[i * 2], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 * i + 1] = evolve_real_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
      evolve_real_elitist_policy_replacement (population, offspring);
      free (offspring);
      evolve_apply_real_pop_fitness (population, fitness_function);

      /* TODO Achar um melhor lugar para colocar essa mutação */
      if (global_options->mutation == regenerated || global_options->mutation == regenerated_permutation)
        evolve_random_generated_individuals (population, REGENERATED_PERCENTAGE, is_valid);
      evolve_apply_real_pop_fitness (population, fitness_function);
      /* TODO Achar um melhor lugar para colocar essa mutação */

      evolve_reset_real_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}
