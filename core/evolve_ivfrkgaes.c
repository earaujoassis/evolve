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
#include "evolve_real_rec.h"
#include "evolve_real_mut.h"
#include "evolve_real_rep.h"
#include "evolve_randomkey.h"
#include "evolve_o_rkgaes.h"
#include "evolve_permutation.h"
#include "evolve_rkgaes.h"
#include "evolve_ivfrkgaes.h"

static size_t global_number_points;

evolve_real_chrom_t **
evolve_ivfrkgaes_pselection (evolve_real_pop_t *population,
                             size_t num_parents)
{
  size_t i;
  evolve_real_chrom_t **individuals = calloc (population->size, sizeof (evolve_real_chrom_t *));
  evolve_real_chrom_t **parents = calloc (num_parents, sizeof (evolve_real_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  evolve_qsort_real_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < num_parents; i++)
    parents[i] = individuals[i];
  free (individuals);
  return parents;
}

size_t
evolve_ivfrkgaes_number_points (size_t chromosome_size)
{
  size_t number_points;
  number_points = (chromosome_size * IVFGA_CUTOFFS_UP_BOUND) - \
    gsl_rng_uniform_int (rng, chromosome_size * IVFGA_CUTOFFS_BOTTOM_BOUND);
  return number_points;
}

size_t *
evolve_ivfrkgaes_xover_points (size_t chromosome_bound)
{
  size_t i, *points;
  points = (size_t *) calloc (global_number_points + 1, sizeof (size_t));
  points[global_number_points] = chromosome_bound;
  for (i = 0; i < global_number_points; i++)
    {
      size_t begin = i * (chromosome_bound / (global_number_points + 1));
      size_t end = (i + 1) * (chromosome_bound / (global_number_points + 1));
      points[i] = gsl_rng_uniform_int (rng, end - begin) + begin;
    }
  return points;
}

void
evolve_ivfrkgaes_manipulation (evolve_real_chrom_t **parents,
                               size_t num_parents,
                               evolve_real_chrom_t *offspring,
                               double (*fitness_function) (evolve_real_chrom_t *),
                               int (*is_valid) (evolve_real_chrom_t *))
{
  size_t i, birthdate, point, *points;
  evolve_real_chrom_t *super_parent, *parent;
  evolve_real_chrom_t *offspring_one, *offspring_two;
  evolve_real_chrom_t *best;
  super_parent = evolve_copy_real_chrom (parents[0], parents[0]->birthdate);
  birthdate = offspring->birthdate;
  best = NULL;
  do
    {
      for (i = 1; i < num_parents; i++)
        {
          parent = parents[i];
          points = evolve_ivfrkgaes_xover_points (super_parent->size);
          offspring_one = evolve_init_real_chrom (super_parent->size, birthdate);
          do
            evolve_real_npoint_xover (super_parent, parent, offspring_one, global_number_points, points);
          while (!(*is_valid) (offspring_one));
          evolve_apply_real_fitness (offspring_one, fitness_function);
          offspring_two = evolve_init_real_chrom (super_parent->size, birthdate);
          do
            evolve_real_npoint_xover (parent, super_parent, offspring_two, global_number_points, points);
          while (!(*is_valid) (offspring_two));
          evolve_apply_real_fitness (offspring_two, fitness_function);
          free (points);
          if (offspring_one->fitness > offspring_two->fitness)
            {
              if (best == NULL)
                {
                  best = offspring_one;
                  evolve_del_real_chrom (offspring_two);
                  continue;
                }
              else if (offspring_one->fitness > best->fitness)
                {
                  evolve_del_real_chrom (best);
                  evolve_del_real_chrom (offspring_two);
                  best = offspring_one;
                  continue;
                }
              else
                {
                  evolve_del_real_chrom (offspring_one);
                  evolve_del_real_chrom (offspring_two);
                  continue;
                }
            }
          else
            {
              if (best == NULL)
                {
                  best = offspring_two;
                  evolve_del_real_chrom (offspring_one);
                  continue;
                }
              else if (offspring_two->fitness > best->fitness)
                {
                  evolve_del_real_chrom (best);
                  evolve_del_real_chrom (offspring_one);
                  best = offspring_two;
                  continue;
                }
              else
                {
                  evolve_del_real_chrom (offspring_one);
                  evolve_del_real_chrom (offspring_two);
                  continue;
                }
            }
        }
      if (best->fitness <= super_parent->fitness)
        break;
      evolve_del_real_chrom (super_parent);
      super_parent = evolve_copy_real_chrom (best, best->birthdate);
    }
  while (1);
  for (i = 0; i < super_parent->size; i++)
    offspring->vector[i] = super_parent->vector[i];
  evolve_del_real_chrom (super_parent);
  evolve_del_real_chrom (best);
  evolve_apply_real_fitness (offspring, fitness_function);
}

void
evolve_ivfrkgaes_transference (const evolve_real_pop_t *population,
                               evolve_real_chrom_t *manipulated)
{
  size_t worst_point;
  worst_point = evolve_worst_real_chrom_index (population);
  population->individuals[worst_point] = manipulated;
}

evolve_stats_t *
evolve_ivfrkgaes (evolve_real_pop_t *population,
                  size_t generations,
                  double (*fitness_function) (evolve_real_chrom_t *),
                  int (*is_valid) (evolve_real_chrom_t *))
{
  size_t i;
  evolve_stats_t *stats = evolve_init_stats ();
  evolve_apply_real_pop_fitness (population, fitness_function);
  evolve_reset_real_pop_stats (population, stats);
  global_number_points = evolve_ivfrkgaes_number_points (population->individuals[0]->size);
  do
    {
      evolve_real_chrom_t **selection, **offspring;
      evolve_real_chrom_t **super_parents, *embryo;
      offspring = (evolve_real_chrom_t **) calloc (population->size, sizeof (evolve_real_chrom_t *));
      if (global_options->selection == proportional)
        selection = evolve_real_fitness_proportional (population, population->size);
      else if (global_options->selection == tournament)
        selection = evolve_real_tournament_selection (population, TOURNAMENT_SIZE, population->size);
      else if (global_options->selection == ranking)
        selection = evolve_real_linear_ranking (population, population->size);
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[2 * i] = evolve_real_breed (selection[2 * i], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 *  i + 1] = evolve_real_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
      embryo = evolve_init_real_chrom (offspring[0]->size, ++population->birthcounter);
      super_parents = evolve_ivfrkgaes_pselection (population, IVFGA_SUPER_PARENTS);
      evolve_ivfrkgaes_manipulation (super_parents, IVFGA_SUPER_PARENTS, embryo, fitness_function, is_valid);
      free (super_parents);
      evolve_real_elitist_policy_replacement (population, offspring);
      free (offspring);
      evolve_apply_real_pop_fitness (population, fitness_function);

      /* TODO Achar um melhor lugar para colocar essa mutação */
      if (global_options->mutation == regenerated || global_options->mutation == regenerated_permutation)
        evolve_random_generated_individuals (population, REGENERATED_PERCENTAGE, is_valid);
      evolve_apply_real_pop_fitness (population, fitness_function);
      /* TODO Achar um melhor lugar para colocar essa mutação */

      evolve_ivfrkgaes_transference (population, embryo);
      evolve_reset_real_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}
