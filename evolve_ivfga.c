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
#include "evolve_stats.h"
#include "evolve_ivfga.h"

evolve_int_chrom_t **
evolve_ivfga_parents_selection (const evolve_object_t *keyword_arguments)
{
  size_t i;
  evolve_int_chrom_t **individuals, **parents;
  evolve_int_pop_t *population = (evolve_int_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t num_parents = *(size_t *) keyword_arguments->get_chain (keyword_arguments, "ivf.num_parents");

  individuals = (evolve_int_chrom_t **) calloc (population->size, sizeof (evolve_int_chrom_t *));
  parents = (evolve_int_chrom_t **) calloc (num_parents, sizeof (evolve_int_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  evolve_qsort_int_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < num_parents; i++)
    parents[i] = individuals[i];
  free (individuals);
  return parents;
}

void
evolve_ivfga_manipulation (const evolve_object_t *keyword_arguments)
{
  double (*fitness) (evolve_int_chrom_t *);
  int (*check_chrom) (evolve_int_chrom_t *);
  void (*recombination_strategy) (const evolve_object_t *);

  size_t i, birthdate;
  evolve_object_t *kw_arguments;
  evolve_int_chrom_t *super_parent, *parent;
  evolve_int_chrom_t *offspring_one, *offspring_two;
  evolve_int_chrom_t *best;
  evolve_int_chrom_t **parents = (evolve_int_chrom_t **) \
    keyword_arguments->get_own_property (keyword_arguments, "parents");
  size_t num_parents = *(size_t *) keyword_arguments->get_chain (keyword_arguments, "ivf.num_parents");
  evolve_int_chrom_t *offspring = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");

  fitness = keyword_arguments->get_own_property (keyword_arguments, "fitness");
  check_chrom = keyword_arguments->get_own_property (keyword_arguments, "check_chrom");
  recombination_strategy = keyword_arguments->get_chain (keyword_arguments, "ivf.recombination_strategy");

  birthdate = offspring->birthdate;
  kw_arguments = evolve_init_object ();
  // The object keyword_arguments.ivf.recombination should exist as well
  kw_arguments->merge (kw_arguments, keyword_arguments->get_own_property (keyword_arguments, "ivf"));
  super_parent = evolve_copy_int_chrom (parents[0], parents[0]->birthdate);
  best = NULL;
  do
    {
      for (i = 1; i < num_parents; i++)
        {
          parent = parents[i];
          offspring_one = evolve_init_int_chrom (super_parent->size, birthdate);
          kw_arguments->set_own_property (kw_arguments, "parent_one", super_parent);
          kw_arguments->set_own_property (kw_arguments, "parent_two", parent);
          kw_arguments->set_own_property (kw_arguments, "offspring", offspring_one);
          do
            recombination_strategy (kw_arguments);
          while (!check_chrom (offspring_one));
          evolve_apply_int_fitness (offspring_one, fitness);
          offspring_two = evolve_init_int_chrom (super_parent->size, birthdate);
          kw_arguments->set_own_property (kw_arguments, "parent_one", parent);
          kw_arguments->set_own_property (kw_arguments, "parent_two", super_parent);
          kw_arguments->set_own_property (kw_arguments, "offspring", offspring_two);
          do
            recombination_strategy (kw_arguments);
          while (!check_chrom (offspring_two));
          evolve_apply_int_fitness (offspring_two, fitness);
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
  evolve_del_object (kw_arguments);
  evolve_del_int_chrom (super_parent);
  evolve_del_int_chrom (best);
  evolve_apply_int_fitness (offspring, fitness);
}

void
evolve_ivfga_transference (const evolve_object_t *keyword_arguments)
{
  size_t worst_point;
  evolve_int_chrom_t *worst;
  const evolve_int_pop_t *population = (const evolve_int_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  evolve_int_chrom_t *manipulated = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "manipulated");

  worst_point = evolve_worst_int_chrom_index (population);
  worst = population->individuals[worst_point];
  population->individuals[worst_point] = manipulated;
  evolve_del_int_chrom (worst);
}

static evolve_stats_t *
evolve_ivfga_execute (const evolve_object_t *keyword_arguments)
{
  double (*fitness) (evolve_int_chrom_t *);
  evolve_int_chrom_t *(*breed) (const evolve_object_t *);
  evolve_int_chrom_t **(*selection_strategy) (const evolve_object_t *);
  void (*replacement_policy) (const evolve_object_t *);

  size_t i;
  evolve_stats_t *stats;
  evolve_object_t *kw_arguments;
  evolve_int_chrom_t **selection, **offspring;
  evolve_int_chrom_t **super_parents, *embryo;
  evolve_int_pop_t *population = (evolve_int_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t generations = *(size_t *) keyword_arguments->get_own_property (keyword_arguments, "generations");

  fitness = keyword_arguments->get_own_property (keyword_arguments, "fitness");
  breed = keyword_arguments->get_own_property (keyword_arguments, "breed");
  selection_strategy = keyword_arguments->get_chain (keyword_arguments, "strategies.selection_strategy");
  replacement_policy = keyword_arguments->get_chain (keyword_arguments, "strategies.replacement_policy");

  stats = evolve_init_stats ();
  kw_arguments = evolve_init_object ();
  kw_arguments->merge (kw_arguments, keyword_arguments->get_own_property (keyword_arguments, "strategies"));
  kw_arguments->set_own_property (kw_arguments, "fitness", fitness);
  evolve_apply_int_pop_fitness (population, fitness);
  evolve_reset_int_pop_stats (population, stats);
  do
    {
      offspring = (evolve_int_chrom_t **) calloc (population->size, sizeof (evolve_int_chrom_t *));
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
      embryo = evolve_init_int_chrom (offspring[0]->size, ++population->birthcounter);
      kw_arguments->set_own_property (kw_arguments, "population", population);
      super_parents = evolve_ivfga_parents_selection (kw_arguments);
      kw_arguments->set_own_property (kw_arguments, "parents", super_parents);
      kw_arguments->set_own_property (kw_arguments, "offspring", embryo);
      evolve_ivfga_manipulation (kw_arguments);
      free (super_parents);
      kw_arguments->set_own_property (kw_arguments, "population", population);
      kw_arguments->set_own_property (kw_arguments, "offspring", offspring);
      replacement_policy (kw_arguments);
      free (offspring);
      evolve_apply_int_pop_fitness (population, fitness);
      kw_arguments->set_own_property (kw_arguments, "population", population);
      kw_arguments->set_own_property (kw_arguments, "manipulated", embryo);
      evolve_ivfga_transference (kw_arguments);
      evolve_reset_int_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  evolve_del_object (kw_arguments);
  return stats;
}

evolve_ivfga_t *
evolve_ivfga_init ()
{
  evolve_ivfga_t *ivfga_solution = evolve_init_object ();

  if (!ivfga_solution)
    return NULL;
  ivfga_solution->set_own_property (ivfga_solution, "solution", &evolve_ivfga_execute);
  return ivfga_solution;
}
