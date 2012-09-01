// Copyright 2011 Éwerton Assis
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

#ifndef __evolve_ga_es_ivf_h__
#define __evolve_ga_es_ivf_h__

/* Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization) */

#include <evolve/paradigms/hybrid/rkgaes.h>

#define IVFGA_SUPER_PARENTS             population->size * 0.25
#define IVFGA_CUTOFFS_UP_BOUND          0.3
#define IVFGA_CUTOFFS_BOTTOM_BOUND      0.2

static size_t global_number_points;

real_chrom_t **
ivfrkgaes_pselection (real_pop_t *population,
                      size_t num_parents)
{
  size_t i;
  real_chrom_t **individuals = calloc (population->size, sizeof (real_chrom_t *));
  real_chrom_t **parents = calloc (num_parents, sizeof (real_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  qsort_real_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < num_parents; i++)
    parents[i] = individuals[i];
  free (individuals);
  return parents;
}

size_t
ivfrkgaes_number_points (size_t chromosome_size)
{
  size_t number_points;
  number_points = (chromosome_size * IVFGA_CUTOFFS_UP_BOUND) - \
    gsl_rng_uniform_int (rng, chromosome_size * IVFGA_CUTOFFS_BOTTOM_BOUND);
  return number_points;
}

size_t *
ivfrkgaes_xover_points (size_t chromosome_bound)
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
ivfrkgaes_manipulation (real_chrom_t **parents,
                        size_t num_parents,
                        real_chrom_t *offspring,
                        double (*fitness_function) (real_chrom_t *),
                        int (*is_valid) (real_chrom_t *))
{
  size_t i, birthdate, point, *points;
  real_chrom_t *super_parent, *parent;
  real_chrom_t *offspring_one, *offspring_two;
  real_chrom_t *best;
  super_parent = copy_real_chrom (parents[0], parents[0]->birthdate);
  birthdate = offspring->birthdate;  
  best = NULL;
  do
    {
      for (i = 1; i < num_parents; i++)
        {
          parent = parents[i];
          points = ivfrkgaes_xover_points (super_parent->size);
          offspring_one = init_real_chrom (super_parent->size, birthdate);
          do
            npoint_xover (super_parent, parent, offspring_one, global_number_points, points);
          while (!(*is_valid) (offspring_one));
          apply_real_fitness (offspring_one, fitness_function);
          offspring_two = init_real_chrom (super_parent->size, birthdate);
          do
            npoint_xover (parent, super_parent, offspring_two, global_number_points, points);
          while (!(*is_valid) (offspring_two));
          apply_real_fitness (offspring_two, fitness_function);
          free (points);
          if (offspring_one->fitness > offspring_two->fitness)
            {
              if (best == NULL)
                {
                  best = offspring_one;
                  del_real_chrom (offspring_two);
                  continue;
                }
              else if (offspring_one->fitness > best->fitness)
                {
                  del_real_chrom (best);
                  del_real_chrom (offspring_two);
                  best = offspring_one;
                  continue;
                }
              else
                {
                  del_real_chrom (offspring_one);
                  del_real_chrom (offspring_two);
                  continue;
                }
            }
          else
            {
              if (best == NULL)
                {
                  best = offspring_two;
                  del_real_chrom (offspring_one);
                  continue;
                }
              else if (offspring_two->fitness > best->fitness)
                {
                  del_real_chrom (best);
                  del_real_chrom (offspring_one);
                  best = offspring_two;
                  continue;
                }
              else
                {
                  del_real_chrom (offspring_one);
                  del_real_chrom (offspring_two);
                  continue;
                }
            }
        }
      if (best->fitness <= super_parent->fitness)
        break;
      del_real_chrom (super_parent);
      super_parent = copy_real_chrom (best, best->birthdate);
    }
  while (1);
  for (i = 0; i < super_parent->size; i++)
    offspring->vector[i] = super_parent->vector[i];
  del_real_chrom (super_parent);
  del_real_chrom (best);
  apply_real_fitness (offspring, fitness_function);
}

void
ivfrkgaes_transference (const real_pop_t *population,
                        real_chrom_t *manipulated)
{
  size_t worst_point;
  worst_point = worst_real_chrom_index (population);
  population->individuals[worst_point] = manipulated;
}

stats_t *
ivfrkgaes (real_pop_t *population,
           size_t generations,
           double (*fitness_function) (real_chrom_t *),
           int (*is_valid) (real_chrom_t *))
{
  size_t i;
  stats_t *stats = init_stats ();
  apply_real_pop_fitness (population, fitness_function);
  reset_real_pop_stats (population, stats);
  global_number_points = ivfrkgaes_number_points (population->individuals[0]->size);
  do
    {
      real_chrom_t **selection, **offspring;
      real_chrom_t **super_parents, *embryo;
      offspring = (real_chrom_t **) calloc (population->size, sizeof (real_chrom_t *));
      if (global_options->selection == proportional)
        selection = fitness_proportional (population, population->size);
      else if (global_options->selection == tournament)
        selection = tournament_selection (population, TOURNAMENT_SIZE, population->size);
      else if (global_options->selection == ranking)
        selection = linear_ranking (population, population->size);
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[2 * i] = real_breed (selection[2 * i], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 *  i + 1] = real_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
      embryo = init_real_chrom (offspring[0]->size, ++population->birthcounter);
      super_parents = ivfrkgaes_pselection (population, IVFGA_SUPER_PARENTS);
      ivfrkgaes_manipulation (super_parents, IVFGA_SUPER_PARENTS, embryo, fitness_function, is_valid);
      free (super_parents);
      elitist_policy_replacement (population, offspring);
      free (offspring);
      apply_real_pop_fitness (population, fitness_function);

      /* TODO Achar um melhor lugar para colocar essa mutação */
      if (global_options->mutation == regenerated || global_options->mutation == regenerated_permutation)
        random_generated_individuals (population, REGENERATED_PERCENTAGE, is_valid);
      apply_real_pop_fitness (population, fitness_function);
      /* TODO Achar um melhor lugar para colocar essa mutação */

      ivfrkgaes_transference (population, embryo);
      reset_real_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}

#endif

