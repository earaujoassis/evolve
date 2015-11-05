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

#include "evolve_stats.h"
#include "evolve_ev.h"

void
evolve_ev_delta_mutation (const evolve_object_t *keyword_arguments)
{
  size_t i;
  evolve_real_chrom_t *offspring = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");
  double step_size = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.step_size");
  double mutation_rate = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.rate");

  for (i = 0; i < offspring->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < mutation_rate)
      {
        if (gsl_ran_flat (rng, 0, 100) < 50.0)
          offspring->vector[i] += gsl_ran_flat (rng, 0, step_size);
        else
          offspring->vector[i] -= gsl_ran_flat (rng, 0, step_size);
      }
}

void
evolve_ev_gaussian_mutation (const evolve_object_t *keyword_arguments)
{
  size_t i;
  evolve_real_chrom_t *offspring = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");
  double step_size = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.step_size");
  double mutation_rate = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.rate");
  double sigma = step_size / sqrt (2 / M_PI);

  for (i = 0; i < offspring->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < mutation_rate)
      offspring->vector[i] += gsl_ran_gaussian (rng, sigma);
}

evolve_real_chrom_t *
evolve_ev_breed (const evolve_object_t *keyword_arguments)
{
  void (*mutation) (const evolve_object_t *keyword_arguments);
  short int (*check_chrom) (evolve_real_chrom_t *);

  evolve_real_chrom_t *offspring;
  const evolve_real_chrom_t *parent = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent");
  size_t birthdate = *(size_t *) keyword_arguments->get_own_property (keyword_arguments, "birthdate");

  mutation = keyword_arguments->get_own_property (keyword_arguments, "mutation_strategy");
  check_chrom = keyword_arguments->get_own_property (keyword_arguments, "check_chrom");

  offspring = evolve_copy_real_chrom (parent, birthdate);
  keyword_arguments->set_own_property (keyword_arguments, "offspring", offspring);
  mutation (keyword_arguments);
  if (!check_chrom (offspring))
    {
      evolve_del_real_chrom (offspring);
      offspring = evolve_copy_real_chrom (parent, birthdate);
    }
  return offspring;
}

static evolve_stats_t *
evolve_ev_execute (evolve_object_t *keyword_arguments)
{
  double (*fitness) (evolve_real_chrom_t *);
  evolve_real_chrom_t *(*breed) (const evolve_object_t *);

  size_t random_individual, pop_size;
  evolve_stats_t *stats;
  evolve_object_t *kw_arguments;
  evolve_real_chrom_t *parent, *offspring, *victim;
  evolve_real_pop_t *population = (evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t generations = *(size_t *) keyword_arguments->get_own_property (keyword_arguments, "generations");

  fitness = keyword_arguments->get_own_property (keyword_arguments, "fitness");
  breed = keyword_arguments->get_own_property (keyword_arguments, "breed");

  stats = evolve_init_stats ();
  kw_arguments = evolve_init_object ();
  kw_arguments->merge (kw_arguments, keyword_arguments->get_own_property (keyword_arguments, "strategies"));
  evolve_apply_real_pop_fitness (population, fitness);
  evolve_reset_real_pop_stats (population, stats);
  do
    {
      pop_size = population->size;

      random_individual = gsl_rng_uniform_int (rng, pop_size);
      parent = population->individuals[random_individual];
      population->birthcounter++;

      kw_arguments->set_own_property (kw_arguments, "parent", parent);
      kw_arguments->set_own_property (kw_arguments, "birthdate", &population->birthcounter);

      offspring = (evolve_real_chrom_t *) breed (kw_arguments);

      offspring->fitness = fitness (offspring);
      random_individual = gsl_rng_uniform_int (rng, pop_size);
      victim = population->individuals[random_individual];
      if (offspring->fitness > victim->fitness)
        {
          population->individuals[random_individual] = offspring;
          evolve_del_real_chrom (victim);
        }
      else
        evolve_del_real_chrom (offspring);
      population->generation++;
      evolve_reset_real_pop_stats (population, stats);
    }
  while (population->generation < generations);
  evolve_del_object (kw_arguments);
  return stats;
}

evolve_ev_t *
evolve_ev_init ()
{
  evolve_ev_t *ev_solution = evolve_init_object ();

  if (!ev_solution)
    return NULL;
  ev_solution->set_own_property (ev_solution, "solution", &evolve_ev_execute);
  return ev_solution;
}
