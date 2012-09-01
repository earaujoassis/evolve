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

#ifndef __evolve_ga_ivf_h__
#define __evolve_ga_ivf_h__

#include <evolve/repr/sorting.h>
#include <evolve/paradigms/ga.h>

#define IVFGA_DEFAULT_XOVER_POINTS      10
#define IVFGA_SUPER_PARENTS             population->size * 0.5

int_chrom_t **
ivfga_pselection (int_pop_t *population,
                  size_t num_parents)
{
  size_t i;
  int_chrom_t **individuals = calloc (population->size, sizeof (int_chrom_t *));
  int_chrom_t **parents = calloc (num_parents, sizeof (int_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  qsort_int_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < num_parents; i++)
    parents[i] = individuals[i];
  free (individuals);
  return parents;
}

void
ivfga_manipulation (int_chrom_t **parents,
                    size_t num_parents,
                    int_chrom_t *offspring,
                    double (*fitness_function) (int_chrom_t *),
                    int (*is_valid) (int_chrom_t *))
{
  size_t i, birthdate = offspring->birthdate;
  int_chrom_t *super_parent, *parent;
  int_chrom_t *offspring_one, *offspring_two;
  int_chrom_t *best;
  super_parent = copy_int_chrom (parents[0], parents[0]->birthdate);
  best = NULL;
  do
    {
      for (i = 1; i < num_parents; i++)
        {
          parent = parents[i];
          offspring_one = init_int_chrom (super_parent->size, birthdate);
          do
            npoint_xover (super_parent, parent, offspring_one, IVFGA_DEFAULT_XOVER_POINTS);
          while (!(*is_valid) (offspring_one));
          apply_int_fitness (offspring_one, fitness_function);
          offspring_two = init_int_chrom (super_parent->size, birthdate);
          do
            npoint_xover (parent, super_parent, offspring_two, IVFGA_DEFAULT_XOVER_POINTS);
          while (!(*is_valid) (offspring_two));
          apply_int_fitness (offspring_two, fitness_function);
          if (offspring_one->fitness > offspring_two->fitness)
            {
              if (best == NULL)
                {
                  best = offspring_one;
                  del_int_chrom (offspring_two);
                  continue;
                }
              else if (offspring_one->fitness > best->fitness)
                {
                  del_int_chrom (best);
                  del_int_chrom (offspring_two);
                  best = offspring_one;
                  continue;
                }
              else
                {
                  del_int_chrom (offspring_one);
                  del_int_chrom (offspring_two);
                  continue;
                }
            }
          else
            {
              if (best == NULL)
                {
                  best = offspring_two;
                  del_int_chrom (offspring_one);
                  continue;
                }
              else if (offspring_two->fitness > best->fitness)
                {
                  del_int_chrom (best);
                  del_int_chrom (offspring_one);
                  best = offspring_two;
                  continue;
                }
              else
                {
                  del_int_chrom (offspring_one);
                  del_int_chrom (offspring_two);
                  continue;
                }
            }
        }
      if (best->fitness <= super_parent->fitness)
        break;
      del_int_chrom (super_parent);
      super_parent = copy_int_chrom (best, best->birthdate);
    }
  while (1);
  for (i = 0; i < super_parent->size; i++)
    offspring->vector[i] = super_parent->vector[i];
  del_int_chrom (super_parent);
  del_int_chrom (best);
  apply_int_fitness (offspring, fitness_function);
}

void
ivfga_transference (const int_pop_t *population,
                    int_chrom_t *manipulated)
{
  size_t worst_point;
  worst_point = worst_int_chrom_index (population);
  population->individuals[worst_point] = manipulated;
}

stats_t *
int_ivfga (int_pop_t *population,
           size_t generations,
           double (*fitness_function) (int_chrom_t *),
           int (*is_valid) (int_chrom_t *))
{
  size_t i;
  stats_t *stats = init_stats ();
  apply_int_pop_fitness (population, fitness_function);
  reset_int_pop_stats (population, stats);
  do
    {
      int_chrom_t **selection, **offspring;
      int_chrom_t **super_parents, *embryo;
      offspring = (int_chrom_t **) calloc (population->size, sizeof (int_chrom_t *));
#ifdef TOURNAMENT_SIZE
      selection = tournament_selection (population, TOURNAMENT_SIZE, population->size);
#else
      selection = roulette_wheel_selection (population, population->size);
#endif
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[2 * i] = int_breed (selection[2 * i], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 *  i + 1] = int_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
      embryo = init_int_chrom (offspring[0]->size, ++population->birthcounter);
      super_parents = ivfga_pselection (population, IVFGA_SUPER_PARENTS);
      ivfga_manipulation (super_parents, IVFGA_SUPER_PARENTS, embryo, fitness_function, is_valid);
      free (super_parents);
#ifdef ELITIST_POLICY
      elitist_policy_replacement (population, offspring);
#else
      generational_replacement (population, offspring);
#endif
      free (offspring);
      apply_int_pop_fitness (population, fitness_function);
      ivfga_transference (population, embryo);
      reset_int_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}

#endif

