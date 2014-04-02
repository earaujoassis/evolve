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

#include <stdlib.h>

#include "evolve_repr_integer.h"
#include "evolve_repr_sorting.h"
#include "evolve_ga.h"
#include "evolve_ivfga.h"

evolve_int_chrom_t **
evolve_ivfga_pselection (evolve_int_pop_t *population,
                         size_t num_parents)
{
  size_t i;
  evolve_int_chrom_t **individuals = calloc (population->size, sizeof (evolve_int_chrom_t *));
  evolve_int_chrom_t **parents = calloc (num_parents, sizeof (evolve_int_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  evolve_qsort_int_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < num_parents; i++)
    parents[i] = individuals[i];
  free (individuals);
  return parents;
}

void
evolve_ivfga_manipulation (evolve_int_chrom_t **parents,
                           size_t num_parents,
                           evolve_int_chrom_t *offspring,
                           double (*fitness_function) (evolve_int_chrom_t *),
                           int (*is_valid) (evolve_int_chrom_t *))
{
  size_t i, birthdate = offspring->birthdate;
  evolve_int_chrom_t *super_parent, *parent;
  evolve_int_chrom_t *offspring_one, *offspring_two;
  evolve_int_chrom_t *best;
  super_parent = evolve_copy_int_chrom (parents[0], parents[0]->birthdate);
  best = NULL;
  do
    {
      for (i = 1; i < num_parents; i++)
        {
          parent = parents[i];
          offspring_one = evolve_init_int_chrom (super_parent->size, birthdate);
          do
            evolve_int_npoint_xover (super_parent, parent, offspring_one, IVFGA_DEFAULT_XOVER_POINTS);
          while (!(*is_valid) (offspring_one));
          evolve_apply_int_fitness (offspring_one, fitness_function);
          offspring_two = evolve_init_int_chrom (super_parent->size, birthdate);
          do
            evolve_int_npoint_xover (parent, super_parent, offspring_two, IVFGA_DEFAULT_XOVER_POINTS);
          while (!(*is_valid) (offspring_two));
          evolve_apply_int_fitness (offspring_two, fitness_function);
          if (offspring_one->fitness > offspring_two->fitness)
            {
              if (best == NULL)
                {
                  best = offspring_one;
                  evolve_del_int_chrom (offspring_two);
                  continue;
                }
              else if (offspring_one->fitness > best->fitness)
                {
                  evolve_del_int_chrom (best);
                  evolve_del_int_chrom (offspring_two);
                  best = offspring_one;
                  continue;
                }
              else
                {
                  evolve_del_int_chrom (offspring_one);
                  evolve_del_int_chrom (offspring_two);
                  continue;
                }
            }
          else
            {
              if (best == NULL)
                {
                  best = offspring_two;
                  evolve_del_int_chrom (offspring_one);
                  continue;
                }
              else if (offspring_two->fitness > best->fitness)
                {
                  evolve_del_int_chrom (best);
                  evolve_del_int_chrom (offspring_one);
                  best = offspring_two;
                  continue;
                }
              else
                {
                  evolve_del_int_chrom (offspring_one);
                  evolve_del_int_chrom (offspring_two);
                  continue;
                }
            }
        }
      if (best->fitness <= super_parent->fitness)
        break;
      evolve_del_int_chrom (super_parent);
      super_parent = evolve_copy_int_chrom (best, best->birthdate);
    }
  while (1);
  for (i = 0; i < super_parent->size; i++)
    offspring->vector[i] = super_parent->vector[i];
  evolve_del_int_chrom (super_parent);
  evolve_del_int_chrom (best);
  evolve_apply_int_fitness (offspring, fitness_function);
}

void
evolve_ivfga_transference (const evolve_int_pop_t *population,
                           evolve_int_chrom_t *manipulated)
{
  size_t worst_point;
  worst_point = evolve_worst_int_chrom_index (population);
  population->individuals[worst_point] = manipulated;
}

evolve_stats_t *
evolve_int_ivfga (evolve_int_pop_t *population,
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
      evolve_int_chrom_t **super_parents, *embryo;
      offspring = (evolve_int_chrom_t **) calloc (population->size, sizeof (evolve_int_chrom_t *));
#ifdef TOURNAMENT_SIZE
      selection = evolve_int_tournament_selection (population, TOURNAMENT_SIZE, population->size);
#else
      selection = evolve_roulette_wheel_selection (population, population->size);
#endif
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[2 * i] = evolve_int_breed (selection[2 * i], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 *  i + 1] = evolve_int_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
      embryo = evolve_init_int_chrom (offspring[0]->size, ++population->birthcounter);
      super_parents = evolve_ivfga_pselection (population, IVFGA_SUPER_PARENTS);
      evolve_ivfga_manipulation (super_parents, IVFGA_SUPER_PARENTS, embryo, fitness_function, is_valid);
      free (super_parents);
#ifdef ELITIST_POLICY
      evolve_elitist_policy_replacement (population, offspring);
#else
      evolve_generational_replacement (population, offspring);
#endif
      free (offspring);
      evolve_apply_int_pop_fitness (population, fitness_function);
      evolve_ivfga_transference (population, embryo);
      evolve_reset_int_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}
