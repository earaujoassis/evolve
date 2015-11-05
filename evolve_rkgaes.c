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
#include "evolve_stats.h"
#include "evolve_rkgaes.h"

static evolve_stats_t *
evolve_rkgaes_execute (const evolve_object_t *keyword_arguments)
{
  double (*fitness) (evolve_real_chrom_t *);
  evolve_real_chrom_t **(*selection_strategy) (const evolve_object_t *);
  evolve_real_chrom_t *(*breed) (const evolve_object_t *);
  void (*replacement_policy) (const evolve_object_t *);
  void (*regenerate_strategy) (const evolve_object_t *);

  size_t i;
  evolve_stats_t *stats;
  evolve_object_t *kw_arguments;
  evolve_real_chrom_t **selection, **offspring;
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
  evolve_apply_real_pop_fitness (population, fitness);
  evolve_reset_real_pop_stats (population, stats);
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

      evolve_reset_real_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}

evolve_rkgaes_t *
evolve_rkgaes_init ()
{
  evolve_rkgaes_t *rkgaes_solution = evolve_init_object ();

  if (!rkgaes_solution)
    return NULL;
  rkgaes_solution->set_own_property (rkgaes_solution, "solution", &evolve_rkgaes_execute);
  return rkgaes_solution;
}
