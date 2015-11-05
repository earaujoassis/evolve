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

#include "evolve_stats.h"
#include "evolve_ga.h"

void
evolve_ga_generational_replacement (const evolve_object_t *keyword_arguments)
{
  size_t i;
  evolve_int_pop_t *population = (evolve_int_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  evolve_int_chrom_t **offspring = (evolve_int_chrom_t **) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");

  evolve_del_int_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
}

void
evolve_ga_elitist_policy_replacement (const evolve_object_t *keyword_arguments)
{
  size_t i, worst_point;
  evolve_int_chrom_t *best, *worst;
  evolve_int_pop_t *population = (evolve_int_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  evolve_int_chrom_t **offspring = (evolve_int_chrom_t **) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");

  best = evolve_best_int_chrom (population);
  best = evolve_copy_int_chrom (best, best->birthdate);
  evolve_del_int_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
  worst_point = evolve_worst_int_chrom_index (population);
  worst = population->individuals[worst_point];
  population->individuals[worst_point] = best;
  evolve_del_int_chrom (worst);
}

evolve_int_chrom_t **
evolve_ga_tournament_selection (const evolve_object_t *keyword_arguments)
{
  size_t i, j, point;
  evolve_int_chrom_t *local_best, **tournament;
  const evolve_int_pop_t *population = (const evolve_int_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t tournament_size = *(size_t *) \
    keyword_arguments->get_chain (keyword_arguments, "selection.tournament_size");
  size_t total_selection = *(size_t *) \
    keyword_arguments->get_chain (keyword_arguments, "selection.total_size");
  evolve_int_chrom_t **selection = (evolve_int_chrom_t **) calloc (total_selection, sizeof (evolve_int_chrom_t *));

  for (i = 0; i < total_selection; i++)
    {
      tournament = (evolve_int_chrom_t **) calloc (tournament_size, sizeof (evolve_int_chrom_t *));
      for (j = 0; j < tournament_size; j++)
        {
          point = gsl_rng_uniform_int (rng, population->size - 1);
          tournament[j] = population->individuals[point];
        }
      local_best = tournament[0];
      for (j = 1; j < tournament_size; j++)
        if (tournament[j]->fitness > local_best->fitness)
          local_best = tournament[j];
      free (tournament);
      selection[i] = local_best;
    }
  return selection;
}

evolve_int_chrom_t **
evolve_ga_roulette_wheel_selection (const evolve_object_t *keyword_arguments)
{
  size_t i, j;
  double amount, roulette_value;
  double total_fitness = 0;
  const evolve_int_pop_t *population = (const evolve_int_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t total_selection = *(size_t *) \
    keyword_arguments->get_chain (keyword_arguments, "selection.total_size");
  evolve_int_chrom_t **selection = (evolve_int_chrom_t **) calloc (total_selection, sizeof (evolve_int_chrom_t *));

  for (i = 0; i < population->size; i++)
    total_fitness += population->individuals[i]->fitness;
  for (i = 0; i < total_selection; i++)
    {
      amount = 0;
      roulette_value = gsl_ran_flat (rng, 0, total_fitness);
      for (j = 0; j < population->size; j++)
        {
          amount += population->individuals[j]->fitness;
          if (amount >= roulette_value)
            break;
        }
      selection[i] = population->individuals[j];
    }
  return selection;
}

void
evolve_ga_uniform_xover (const evolve_object_t *keyword_arguments)
{
  size_t i;
  double rand;
  const evolve_int_chrom_t *parent_one = (const evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_int_chrom_t *parent_two = (const evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  evolve_int_chrom_t *offspring = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");
  double parameterized = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "recombination.parameterized");

  for (i = 0; i < offspring->size; i++)
    {
      rand = gsl_ran_flat (rng, 0, 100);
      if (rand < parameterized) /* Head */
        offspring->vector[i] = parent_one->vector[i];
      else /* Tails */
        offspring->vector[i] = parent_two->vector[i];
    }
}

void
evolve_ga_onepoint_xover (const evolve_object_t *keyword_arguments)
{
  size_t i, point;
  const evolve_int_chrom_t *parent_one = (const evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_int_chrom_t *parent_two = (const evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  evolve_int_chrom_t *offspring = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");

  point = gsl_rng_uniform_int (rng, offspring->size - 1);
  for (i = 0; i < point; i++)
    offspring->vector[i] = parent_one->vector[i];
  for (i = point; i < offspring->size; i++)
    offspring->vector[i] = parent_two->vector[i];
}

void
evolve_ga_npoint_xover (const evolve_object_t *keyword_arguments)
{
  size_t i, point, *points, deslocation, begin, end;
  const evolve_int_chrom_t *parent_one = (const evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_int_chrom_t *parent_two = (const evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  evolve_int_chrom_t *offspring = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");
  size_t num_points = *(size_t *) \
    keyword_arguments->get_chain (keyword_arguments, "recombination.num_points");

  points = (size_t *) calloc (num_points + 1, sizeof (size_t));
  points[num_points] = parent_one->size;
  for (i = 0; i < num_points; i++)
    {
      begin = i * (parent_one->size / (num_points + 1));
      end = (i + 1) * (parent_one->size / (num_points + 1));
      points[i] = gsl_rng_uniform_int (rng, end - begin) + begin;
    }
  deslocation = 0;
  for (point = 0; point < num_points + 1; point++)
    {
      for (i = deslocation; i < points[point]; i++)
        if (point % 2)
          offspring->vector[i] = parent_one->vector[i];
        else
          offspring->vector[i] = parent_two->vector[i];
      deslocation = points[point];
    }
  free (points);
}

void
evolve_ga_per_int_mutation (const evolve_object_t *keyword_arguments)
{
  size_t i;
  evolve_int_chrom_t *offspring = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");
  double mutation_chance = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.chance");

  for (i = 0; i < offspring->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < mutation_chance)
      offspring->vector[i] = offspring->vector[i] ? 0 : 1;
}

evolve_int_chrom_t *
evolve_ga_breed (const evolve_object_t *keyword_arguments)
{
  short int (*check_chrom) (evolve_int_chrom_t *);
  void (*recombination_strategy) (const evolve_object_t *);
  void (*mutation_strategy) (const evolve_object_t *);

  evolve_int_chrom_t *offspring;
  const evolve_int_chrom_t *parent_one = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_int_chrom_t *parent_two = (evolve_int_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  size_t birthdate = *(size_t *) keyword_arguments->get_own_property (keyword_arguments, "birthdate");
  double xover_rate = *(double *) keyword_arguments->get_chain (keyword_arguments, "recombination.xover_rate");

  check_chrom = keyword_arguments->get_own_property (keyword_arguments, "check_chrom");
  recombination_strategy = keyword_arguments->get_own_property (keyword_arguments, "recombination_strategy");
  mutation_strategy = keyword_arguments->get_own_property (keyword_arguments, "mutation_strategy");

  offspring = evolve_init_int_chrom (parent_one->size, birthdate);
  keyword_arguments->set_own_property (keyword_arguments, "offspring", offspring);
  do
    {
      if (gsl_ran_flat (rng, 0, 100) < xover_rate)
        recombination_strategy (keyword_arguments);
      if (mutation_strategy != NULL)
        mutation_strategy (keyword_arguments);
    }
  while (!check_chrom (offspring));
  return offspring;
}

static evolve_stats_t *
evolve_ga_execute (const evolve_object_t *keyword_arguments)
{
  double (*fitness) (evolve_int_chrom_t *);
  evolve_int_chrom_t *(*breed) (const evolve_object_t *);
  evolve_int_chrom_t **(*selection_strategy) (const evolve_object_t *);
  void (*replacement_policy) (const evolve_object_t *);

  size_t i;
  evolve_stats_t *stats;
  evolve_object_t *kw_arguments;
  evolve_int_chrom_t **selection, **offspring;
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
      kw_arguments->set_own_property (kw_arguments, "population", population);
      kw_arguments->set_own_property (kw_arguments, "offspring", offspring);
      replacement_policy (kw_arguments);
      free (offspring);
      evolve_apply_int_pop_fitness (population, fitness);
      evolve_reset_int_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  evolve_del_object (kw_arguments);
  return stats;
}

evolve_ga_t *
evolve_ga_init ()
{
  evolve_ga_t *ga_solution = evolve_init_object ();

  if (!ga_solution)
    return NULL;
  ga_solution->set_own_property (ga_solution, "solution", &evolve_ga_execute);
  return ga_solution;
}
