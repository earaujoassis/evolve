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

#include <stdlib.h>

#include "evolve_repr_sorting.h"
#include "evolve_randomkey.h"
#include "evolve_permutation.h"
#include "evolve_stats.h"
#include "evolve_rkivfgaes.h"

static size_t global_number_points;

evolve_real_chrom_t **
evolve_rkivfgaes_parents_selection (const evolve_object_t *keyword_arguments)
{
  size_t i;
  evolve_real_chrom_t **individuals, **parents;
  evolve_real_pop_t *population = (evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t num_parents = *(size_t *) keyword_arguments->get_chain (keyword_arguments, "ivf.num_parents");

  individuals = calloc (population->size, sizeof (evolve_real_chrom_t *));
  parents = calloc (num_parents, sizeof (evolve_real_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  evolve_qsort_real_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < num_parents; i++)
    parents[i] = individuals[i];
  free (individuals);
  return parents;
}

static size_t
evolve_rkivfgaes_number_points (size_t chromosome_size,
                                double cutoffs_up_bound,
                                double cutoffs_bottom_bound)
{
  size_t number_points;

  number_points = (chromosome_size * cutoffs_up_bound) - \
    gsl_rng_uniform_int (rng, chromosome_size * cutoffs_bottom_bound);
  return number_points;
}

static size_t *
evolve_rkivfgaes_xover_points (size_t chromosome_bound)
{
  size_t i, *points, begin, end;

  points = (size_t *) calloc (global_number_points + 1, sizeof (size_t));
  points[global_number_points] = chromosome_bound;
  for (i = 0; i < global_number_points; i++)
    {
      begin = i * (chromosome_bound / (global_number_points + 1));
      end = (i + 1) * (chromosome_bound / (global_number_points + 1));
      points[i] = gsl_rng_uniform_int (rng, end - begin) + begin;
    }
  return points;
}

void
evolve_rkivfgaes_manipulation (const evolve_object_t *keyword_arguments)
{
  double (*fitness) (evolve_real_chrom_t *);
  int (*check_chrom) (evolve_real_chrom_t *);
  void (*recombination_strategy) (const evolve_object_t *);

  size_t i, birthdate, point, *points;
  evolve_object_t *kw_arguments;
  evolve_real_chrom_t *super_parent, *parent;
  evolve_real_chrom_t *offspring_one, *offspring_two;
  evolve_real_chrom_t *best;
  evolve_real_chrom_t **parents = (evolve_real_chrom_t **) \
    keyword_arguments->get_own_property (keyword_arguments, "parents");
  size_t num_parents = *(size_t *) keyword_arguments->get_chain (keyword_arguments, "ivf.num_parents");
  evolve_real_chrom_t *offspring = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");

  fitness = keyword_arguments->get_own_property (keyword_arguments, "fitness");
  check_chrom = keyword_arguments->get_own_property (keyword_arguments, "check_chrom");
  recombination_strategy = keyword_arguments->get_own_property (keyword_arguments, "ivf.recombination_strategy");

  kw_arguments = evolve_init_object ();
  // The object keyword_arguments.ivf.recombination should exist as well
  kw_arguments->merge (kw_arguments, keyword_arguments->get_own_property (keyword_arguments, "ivf"));
  kw_arguments->set_chain (kw_arguments, "recombination.num_points", &global_number_points);
  super_parent = evolve_copy_real_chrom (parents[0], parents[0]->birthdate);
  birthdate = offspring->birthdate;
  best = NULL;
  do
    {
      for (i = 1; i < num_parents; i++)
        {
          parent = parents[i];
          points = evolve_rkivfgaes_xover_points (super_parent->size);
          offspring_one = evolve_init_real_chrom (super_parent->size, birthdate);
          // kw_arguments->set_chain (kw_arguments, "recombination.selected_points", points);
          kw_arguments->set_own_property (kw_arguments, "parent_one", super_parent);
          kw_arguments->set_own_property (kw_arguments, "parent_two", parent);
          kw_arguments->set_own_property (kw_arguments, "offspring", offspring_one);
          do
            recombination_strategy (kw_arguments);
          while (!check_chrom (offspring_one));
          evolve_apply_real_fitness (offspring_one, fitness);
          offspring_two = evolve_init_real_chrom (super_parent->size, birthdate);
          kw_arguments->set_own_property (kw_arguments, "parent_one", parent);
          kw_arguments->set_own_property (kw_arguments, "parent_two", super_parent);
          kw_arguments->set_own_property (kw_arguments, "offspring", offspring_two);
          do
            recombination_strategy (kw_arguments);
          while (!check_chrom (offspring_two));
          evolve_apply_real_fitness (offspring_two, fitness);
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
  evolve_del_object (kw_arguments);
  evolve_del_real_chrom (super_parent);
  evolve_del_real_chrom (best);
  evolve_apply_real_fitness (offspring, fitness);
}

void
evolve_rkivfgaes_transference (const evolve_object_t *keyword_arguments)
{
  size_t worst_point;
  evolve_real_chrom_t *worst;
  const evolve_real_pop_t *population = (const evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  evolve_real_chrom_t *manipulated = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "manipulated");

  worst_point = evolve_worst_real_chrom_index (population);
  worst = population->individuals[worst_point];
  population->individuals[worst_point] = manipulated;
  evolve_del_real_chrom (worst);
}

static evolve_stats_t *
evolve_rkivfgaes_execute (const evolve_object_t *keyword_arguments)
{
  double (*fitness) (evolve_real_chrom_t *);
  evolve_real_chrom_t **(*selection_strategy) (const evolve_object_t *);
  evolve_real_chrom_t *(*breed) (const evolve_object_t *);
  void (*replacement_policy) (const evolve_object_t *);
  void (*regenerate_strategy) (const evolve_object_t *);

  size_t i;
  double cutoffs_up_bound, cutoffs_bottom_bound;
  evolve_stats_t *stats;
  evolve_object_t *kw_arguments;
  evolve_real_chrom_t **selection, **offspring;
  evolve_real_chrom_t **super_parents, *embryo;
  evolve_real_pop_t *population = (evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t generations = *(size_t *) keyword_arguments->get_own_property (keyword_arguments, "generations");

  fitness = keyword_arguments->get_own_property (keyword_arguments, "fitness");
  breed = keyword_arguments->get_own_property (keyword_arguments, "breed");
  selection_strategy = keyword_arguments->get_chain (keyword_arguments, "strategies.selection_strategy");
  replacement_policy = keyword_arguments->get_chain (keyword_arguments, "strategies.replacement_policy");
  regenerate_strategy = keyword_arguments->get_chain (keyword_arguments, "strategies.regenerate_strategy");

  stats = evolve_init_stats ();
  kw_arguments = evolve_init_object ();
  kw_arguments->merge (kw_arguments, keyword_arguments->get_own_property (keyword_arguments, "strategies"));
  kw_arguments->set_own_property (kw_arguments, "fitness", fitness);
  cutoffs_up_bound = *(double *) kw_arguments->get_chain (kw_arguments, "ivf.cutoffs_up_bound");
  cutoffs_bottom_bound = *(double *) kw_arguments->get_chain (kw_arguments, "ivf.cutoffs_bottom_bound");
  evolve_apply_real_pop_fitness (population, fitness);
  evolve_reset_real_pop_stats (population, stats);
  global_number_points = evolve_rkivfgaes_number_points (population->individuals[0]->size, \
    cutoffs_up_bound, cutoffs_bottom_bound);
  do
    {
      offspring = (evolve_real_chrom_t **) calloc (population->size, sizeof (evolve_real_chrom_t *));
      kw_arguments->set_own_property (kw_arguments, "population", population);
      selection = selection_strategy (kw_arguments);
      for (i = 0; i < population->size / 2; i++)
        {
          kw_arguments->set_own_property (kw_arguments, "parent_one", selection[2 * i]);
          kw_arguments->set_own_property (kw_arguments, "parent_two", selection[2 * i + 1]);
          ++population->birthcounter;
          kw_arguments->set_own_property (kw_arguments, "birthdate", &(population->birthcounter));

          offspring[2 * i] = breed (kw_arguments);

          kw_arguments->set_own_property (kw_arguments, "parent_one", selection[2 * i + 1]);
          kw_arguments->set_own_property (kw_arguments, "parent_two", selection[2 * i]);
          ++population->birthcounter;
          kw_arguments->set_own_property (kw_arguments, "birthdate", &(population->birthcounter));

          offspring[2 * i + 1] = breed (kw_arguments);
        }
      free (selection);
      embryo = evolve_init_real_chrom (offspring[0]->size, ++population->birthcounter);
      kw_arguments->set_own_property (kw_arguments, "population", population);
      super_parents = evolve_rkivfgaes_parents_selection (kw_arguments);
      kw_arguments->set_own_property (kw_arguments, "parents", super_parents);
      kw_arguments->set_own_property (kw_arguments, "offspring", embryo);
      evolve_rkivfgaes_manipulation (kw_arguments);
      free (super_parents);
      kw_arguments->set_own_property (kw_arguments, "population", population);
      kw_arguments->set_own_property (kw_arguments, "offspring", offspring);
      replacement_policy (kw_arguments);
      free (offspring);
      evolve_apply_real_pop_fitness (population, fitness);

      /* TODO Find a better place to call this mutation */
      if (regenerate_strategy != NULL)
        {
          kw_arguments->set_own_property (kw_arguments, "population", population);
          regenerate_strategy (kw_arguments);
          evolve_apply_real_pop_fitness (population, fitness);
        }

      kw_arguments->set_own_property (kw_arguments, "population", population);
      kw_arguments->set_own_property (kw_arguments, "manipulated", embryo);
      evolve_rkivfgaes_transference (kw_arguments);
      evolve_reset_real_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}

evolve_rkivfgaes_t *
evolve_rkivfgaes_init ()
{
  evolve_rkivfgaes_t *rkivfgaes_solution = evolve_init_object ();

  if (!rkivfgaes_solution)
    return NULL;
  rkivfgaes_solution->set_own_property (rkivfgaes_solution, "solution", &evolve_rkivfgaes_execute);
  return rkivfgaes_solution;
}
