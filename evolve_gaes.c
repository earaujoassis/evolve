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

#include "evolve_permutation.h"
#include "evolve_repr_sorting.h"
#include "evolve_gaes.h"

void
evolve_gaes_elitist_policy_replacement (const evolve_object_t *keyword_arguments)
{
  size_t i, worst_point;
  evolve_real_chrom_t *best, *worst;
  evolve_real_pop_t *population = (evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  evolve_real_chrom_t **offspring = (evolve_real_chrom_t **) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");

  best = evolve_best_real_chrom (population);
  best = evolve_copy_real_chrom (best, best->birthdate);
  evolve_del_real_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
  worst_point = evolve_worst_real_chrom_index (population);
  worst = population->individuals[worst_point];
  population->individuals[worst_point] = best;
  evolve_del_real_chrom (worst);
}

evolve_real_chrom_t **
evolve_gaes_linear_ranking (const evolve_object_t *keyword_arguments)
{
  size_t i;
  evolve_real_chrom_t **individuals, **selection;
  const evolve_real_pop_t *population = (const evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t total_selection = *(size_t *) \
    keyword_arguments->get_chain (keyword_arguments, "selection.total_size");

  individuals = calloc (population->size, sizeof (evolve_real_chrom_t *));
  selection = calloc (total_selection, sizeof (evolve_real_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  evolve_qsort_real_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < total_selection; i++)
    selection[i] = individuals[i % 2];
  free (individuals);
  return selection;
}

evolve_real_chrom_t **
evolve_gaes_tournament_selection (const evolve_object_t *keyword_arguments)
{
  size_t i, j, point;
  evolve_real_chrom_t *local_best, **selection;
  const evolve_real_pop_t *population = (const evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t tournament_size = *(size_t *) keyword_arguments->get_chain (keyword_arguments, "selection.tournament_size");
  size_t total_selection = *(size_t *) keyword_arguments->get_chain (keyword_arguments, "selection.total_size");

  selection = (evolve_real_chrom_t **) calloc (total_selection, sizeof (evolve_real_chrom_t *));
  for (i = 0; i < total_selection; i++)
    {
      evolve_real_chrom_t **tournament = (evolve_real_chrom_t **) calloc (tournament_size, sizeof (evolve_real_chrom_t *));
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

evolve_real_chrom_t **
evolve_gaes_fitness_proportional (const evolve_object_t *keyword_arguments)
{
  size_t i, j;
  double amount, roulette_value;
  double total_fitness = 0;
  evolve_real_chrom_t **selection;
  const evolve_real_pop_t *population = (const evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  size_t total_selection = *(size_t *) keyword_arguments->get_chain (keyword_arguments, "selection.total_size");

  selection = (evolve_real_chrom_t **) calloc (total_selection, sizeof (evolve_real_chrom_t *));
  for (i = 0; i < population->size; i++)
    total_fitness -= population->individuals[i]->fitness;
  for (i = 0; i < total_selection; i++)
    {
      amount = 0;
      roulette_value = gsl_ran_flat (rng, 0, total_fitness);
      for (j = 0; j < population->size; j++)
        {
          amount -= population->individuals[j]->fitness;
          if (amount >= roulette_value)
            break;
        }
      selection[i] = population->individuals[j];
    }
  return selection;
}

void
evolve_gaes_uniform_xover (const evolve_object_t *keyword_arguments)
{
  size_t i;
  double rand;
  const evolve_real_chrom_t *parent_one = (const evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_real_chrom_t *parent_two = (const evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  evolve_real_chrom_t *offspring = (evolve_real_chrom_t *) \
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
evolve_gaes_onepoint_xover (const evolve_object_t *keyword_arguments)
{
  size_t i, point;
  const evolve_real_chrom_t *parent_one = (const evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_real_chrom_t *parent_two = (const evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  evolve_real_chrom_t *offspring = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");

  point = gsl_rng_uniform_int (rng, offspring->size - 1);
  for (i = 0; i < point; i++)
    offspring->vector[i] = parent_one->vector[i];
  for (i = point; i < offspring->size; i++)
    offspring->vector[i] = parent_two->vector[i];
}

void
evolve_gaes_npoint_xover (const evolve_object_t *keyword_arguments)
{
  size_t i, point, *points, deslocation, begin, end;
  const evolve_real_chrom_t *parent_one = (const evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_real_chrom_t *parent_two = (const evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  evolve_real_chrom_t *offspring = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");
  size_t num_points = *(size_t *) \
    keyword_arguments->get_chain (keyword_arguments, "recombination.num_points");
  size_t *selected_points = (size_t *) \
    keyword_arguments->get_chain (keyword_arguments, "recombination.selected_points");

  if (selected_points == NULL)
    {
      points = (size_t *) calloc (num_points + 1, sizeof (size_t));
      points[num_points] = parent_one->size;
      for (i = 0; i < num_points; i++)
        {
          begin = i * (parent_one->size / (num_points + 1));
          end = (i + 1) * (parent_one->size / (num_points + 1));
          points[i] = gsl_rng_uniform_int (rng, end - begin) + begin;
        }
    }
  else
    points = selected_points;
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
  if (selected_points == NULL)
    free (points);
}

void
evolve_gaes_random_generated_individuals (const evolve_object_t *keyword_arguments)
{
  int (*check_chrom) (evolve_real_chrom_t *);

  size_t i, j, amount, dimension, chromosome_size;
  evolve_real_chrom_t *individual;
  evolve_real_pop_t *population = (evolve_real_pop_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "population");
  double population_percentage = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "regenerate.population_percentage");
  check_chrom = keyword_arguments->get_own_property (keyword_arguments, "check_chrom");

  chromosome_size = population->individuals[0]->size;
  amount = population->size * population_percentage;
  dimension = population->size / 2;
  evolve_qsort_real_chrom (population->individuals, 0, population->size - 1);
  for (i = population->size - amount; i < population->size; i++)
    {
      individual = population->individuals[i];
      evolve_del_real_chrom (individual);
      population->birthcounter++;
      individual = evolve_init_real_chrom (chromosome_size, population->birthcounter);
      do
        for (j = 0; j < chromosome_size; j++)
          individual->vector[j] = gsl_ran_flat (rng, 0, dimension);
      while (!check_chrom (individual));
      population->individuals[i] = individual;
    }
}

void
evolve_gaes_permutation_mutation (const evolve_object_t *keyword_arguments)
{
  evolve_real_chrom_t *offspring = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "offspring");
  double prob_exchange = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.prob_exchange");
  double prob_insertion = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.prob_insertion");
  double prob_invertion = *(double *) \
    keyword_arguments->get_chain (keyword_arguments, "mutation.prob_invertion");
  double rand = gsl_ran_flat (rng, 0, 100);

  if (rand <= prob_exchange)
    evolve_exchange_operator (offspring);
  else if (prob_exchange < rand && rand <= prob_exchange + prob_insertion)
    evolve_insertion_operator (offspring);
  else if (prob_exchange + prob_insertion < rand \
    && rand < prob_exchange + prob_insertion + prob_invertion)
    evolve_inversion_operator (offspring);
  else
    evolve_shifting_operator (offspring);
}

evolve_real_chrom_t *
evolve_gaes_breed (const evolve_object_t *keyword_arguments)
{
  int (*check_chrom) (evolve_real_chrom_t *);
  void (*recombination_strategy) (const evolve_object_t *);
  void (*mutation_strategy) (const evolve_object_t *);

  evolve_real_chrom_t *offspring;
  const evolve_real_chrom_t *parent_one = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_one");
  const evolve_real_chrom_t *parent_two = (evolve_real_chrom_t *) \
    keyword_arguments->get_own_property (keyword_arguments, "parent_two");
  size_t birthdate = *(size_t *) keyword_arguments->get_own_property (keyword_arguments, "birthdate");

  check_chrom = keyword_arguments->get_own_property (keyword_arguments, "check_chrom");
  recombination_strategy = keyword_arguments->get_own_property (keyword_arguments, "recombination_strategy");
  mutation_strategy = keyword_arguments->get_own_property (keyword_arguments, "mutation_strategy");

  offspring = evolve_init_real_chrom (parent_one->size, birthdate);
  keyword_arguments->set_own_property (keyword_arguments, "offspring", offspring);
  do
    {
      recombination_strategy (keyword_arguments);
      if (mutation_strategy != NULL)
        mutation_strategy (keyword_arguments);
    }
  while (!check_chrom (offspring));
  return offspring;
}
