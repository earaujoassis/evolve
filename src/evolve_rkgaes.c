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

#include "evolve_rng.h"
#include "evolve_repr_sorting.h"
#include "evolve_o_rkgaes.h"
#include "evolve_permutation.h"
#include "evolve_rkgaes.h"
#include "evolve_randomkey.h"

void
evolve_real_uniform_xover (const evolve_real_chrom_t *parent_one,
                           const evolve_real_chrom_t *parent_two,
                           evolve_real_chrom_t *offspring,
                           double parameterized)
{
  double rand;
  size_t i;
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
evolve_real_onepoint_xover (const evolve_real_chrom_t *parent_one,
                            const evolve_real_chrom_t *parent_two,
                            evolve_real_chrom_t *offspring)
{
  size_t i;
  size_t point = gsl_rng_uniform_int (rng, offspring->size - 1);
  for (i = 0; i < point; i++)
    offspring->vector[i] = parent_one->vector[i];
  for (i = point; i < offspring->size; i++)
    offspring->vector[i] = parent_two->vector[i];
}

void
evolve_real_npoint_xover (const evolve_real_chrom_t *parent_one,
                          const evolve_real_chrom_t *parent_two,
                          evolve_real_chrom_t *offspring,
                          size_t num_points,
                          size_t *selected_points)
{
  size_t i, point, *points, deslocation;
  if (selected_points == NULL)
    {
      points = (size_t *) calloc (num_points + 1, sizeof (size_t));
      points[num_points] = parent_one->size;
      for (i = 0; i < num_points; i++)
        {
          size_t begin = i * (parent_one->size / (num_points + 1));
          size_t end = (i + 1) * (parent_one->size / (num_points + 1));
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
evolve_permutation_mutation (evolve_real_chrom_t *offspring)
{
  double rand = gsl_ran_flat (rng, 0, 100);
  if (rand <= PROB_EXCHANGE)
    evolve_exchange_operator (offspring);
  else if (PROB_EXCHANGE < rand && rand <= PROB_EXCHANGE + PROB_INSERTION)
    evolve_insertion_operator (offspring);
  else if (PROB_EXCHANGE + PROB_INSERTION < rand 
    && rand < PROB_EXCHANGE + PROB_INSERTION + PROB_INVERSION)
    evolve_inversion_operator (offspring);
  else
    evolve_shifting_operator (offspring);
}

void
evolve_random_generated_individuals (evolve_real_pop_t *population,
                                     double population_percentage,
                                     int (*is_valid) (evolve_real_chrom_t *))
{
  size_t i, j;
  size_t amount, dimension;
  size_t chromosome_size = population->individuals[0]->size;
  amount = population->size * population_percentage;
  dimension = population->size / 2;
  evolve_qsort_real_chrom (population->individuals, 0, population->size - 1);
  for (i = population->size - amount; i < population->size; i++)
    {
      evolve_real_chrom_t *individual = population->individuals[i];
      evolve_del_real_chrom (individual);
      population->birthcounter++;
      individual = evolve_init_real_chrom (chromosome_size, population->birthcounter);
      do
        for (j = 0; j < chromosome_size; j++)
          individual->vector[j] = gsl_ran_flat (rng, 0, dimension);
      while (!(*is_valid) (individual));
      population->individuals[i] = individual;
    }
}

evolve_real_chrom_t **
evolve_linear_ranking (const evolve_real_pop_t *population,
                       size_t total_selection)
{
  size_t i;
  evolve_real_chrom_t **individuals = calloc (population->size, sizeof (evolve_real_chrom_t *));
  evolve_real_chrom_t **selection = calloc (total_selection, sizeof (evolve_real_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  evolve_qsort_real_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < total_selection; i++)
    selection[i] = individuals[i % 2];
  free (individuals);
  return selection;
}

evolve_real_chrom_t **
evolve_real_tournament_selection (const evolve_real_pop_t *population,
                                  size_t tournament_size,
                                  size_t total_selection)
{
  size_t i, j;
  evolve_real_chrom_t *local_best;
  evolve_real_chrom_t **selection = (evolve_real_chrom_t **) calloc (total_selection, sizeof (evolve_real_chrom_t *));
  for (i = 0; i < total_selection; i++)
    {
      evolve_real_chrom_t **tournament = (evolve_real_chrom_t **) calloc (tournament_size, sizeof (evolve_real_chrom_t *));
      for (j = 0; j < tournament_size; j++)
        {
          size_t point = gsl_rng_uniform_int (rng, population->size - 1);
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
evolve_fitness_proportional (const evolve_real_pop_t *population,
                             size_t total_selection)
{
  size_t i, j;
  double amount, roulette_value;
  double total_fitness = 0;
  evolve_real_chrom_t **selection = (evolve_real_chrom_t **) calloc (total_selection, sizeof (evolve_real_chrom_t *));
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

evolve_real_chrom_t *
evolve_real_breed (const evolve_real_chrom_t *parent_one,
                   const evolve_real_chrom_t *parent_two,
                   size_t birthdate,
                   int (*is_valid) (evolve_real_chrom_t *))
{
  evolve_real_chrom_t *offspring = evolve_init_real_chrom (parent_one->size, birthdate);
  do
    {
      if (global_options->recombination == onepoint)
        evolve_real_onepoint_xover (parent_one, parent_two, offspring);
      else if (global_options->recombination == npoint)
        evolve_real_npoint_xover (parent_one, parent_two, offspring, NPOINTS_XOVER, NULL);
      else if (global_options->recombination == uniform)
        evolve_real_uniform_xover (parent_one, parent_two, offspring, HEAD_TOSSING);
      if (global_options->mutation == permutation || global_options->mutation == regenerated_permutation)
        evolve_permutation_mutation (offspring);
    }
  while (!(*is_valid) (offspring));
  return offspring;
}

void
evolve_elitist_policy_replacement (evolve_real_pop_t *population,
                                   evolve_real_chrom_t **offspring)
{
  size_t i, worst_point;
  evolve_real_chrom_t *best;
  best = evolve_best_real_chrom (population);
  best = evolve_copy_real_chrom (best, best->birthdate);
  evolve_del_real_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
  worst_point = evolve_worst_real_chrom_index (population);
  population->individuals[worst_point] = best;
}

evolve_stats_t *
evolve_rkgaes (evolve_real_pop_t *population,
               size_t generations,
               double (*fitness_function) (evolve_real_chrom_t *),
               int (*is_valid) (evolve_real_chrom_t *))
{
  size_t i;
  evolve_stats_t *stats = evolve_init_stats ();
  evolve_apply_real_pop_fitness (population, fitness_function);
  evolve_reset_real_pop_stats (population, stats);
  do
    {
      evolve_real_chrom_t **selection, **offspring;
      offspring = (evolve_real_chrom_t **) calloc (population->size, sizeof (evolve_real_chrom_t *));
      if (global_options->selection == proportional)
        selection = evolve_fitness_proportional (population, population->size);
      else if (global_options->selection == tournament)
        selection = evolve_real_tournament_selection (population, TOURNAMENT_SIZE, population->size);
      else if (global_options->selection == ranking)
        selection = evolve_linear_ranking (population, population->size);
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[i * 2] = evolve_real_breed (selection[i * 2], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 * i + 1] = evolve_real_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
      evolve_elitist_policy_replacement (population, offspring);
      free (offspring);
      evolve_apply_real_pop_fitness (population, fitness_function);

      /* TODO Achar um melhor lugar para colocar essa mutação */
      if (global_options->mutation == regenerated || global_options->mutation == regenerated_permutation)
        evolve_random_generated_individuals (population, REGENERATED_PERCENTAGE, is_valid);
      evolve_apply_real_pop_fitness (population, fitness_function);
      /* TODO Achar um melhor lugar para colocar essa mutação */

      evolve_reset_real_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}
