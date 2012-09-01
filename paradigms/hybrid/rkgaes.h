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

#ifndef __evolve_hybrid_real_ga_es_h__
#define __evolve_hybrid_real_ga_es_h__

/* Random-key Genetic Algorithm/Evolution Strategy */

#include <evolve/paradigms/es.h>
#include <evolve/repr/sorting.h>
#include <evolve/utils/options/rkgaes.h>
#include "randomkey.h"
#include "permutation.h"

#define TOURNAMENT_SIZE         5
#define NPOINTS_XOVER           4
#define HEAD_TOSSING            70.0
#define REGENERATED_PERCENTAGE  0.20

void
uniform_xover (const real_chrom_t *parent_one,
               const real_chrom_t *parent_two,
               real_chrom_t *offspring,
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
onepoint_xover (const real_chrom_t *parent_one,
                const real_chrom_t *parent_two,
                real_chrom_t *offspring)
{
  size_t i;
  size_t point = gsl_rng_uniform_int (rng, offspring->size - 1);
  for (i = 0; i < point; i++)
    offspring->vector[i] = parent_one->vector[i];
  for (i = point; i < offspring->size; i++)
    offspring->vector[i] = parent_two->vector[i];
}

void
npoint_xover (const real_chrom_t *parent_one,
              const real_chrom_t *parent_two,
              real_chrom_t *offspring,
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
permutation_mutation (real_chrom_t *offspring)
{
  double rand = gsl_ran_flat (rng, 0, 100);
  if (rand <= PROB_EXCHANGE)
    exchange_operator (offspring);
  else if (PROB_EXCHANGE < rand && rand <= PROB_EXCHANGE + PROB_INSERTION)
    insertion_operator (offspring);
  else if (PROB_EXCHANGE + PROB_INSERTION < rand 
    && rand < PROB_EXCHANGE + PROB_INSERTION + PROB_INVERSION)
    inversion_operator (offspring);
  else
    shifting_operator (offspring);
}

void
random_generated_individuals (real_pop_t *population,
                              double population_percentage,
                              int (*is_valid) (real_chrom_t *))
{
  size_t i, j;
  size_t amount, dimension;
  size_t chromosome_size = population->individuals[0]->size;
  amount = population->size * population_percentage;
  dimension = population->size / 2;
  qsort_real_chrom (population->individuals, 0, population->size - 1);
  for (i = population->size - amount; i < population->size; i++)
    {
      real_chrom_t *individual = population->individuals[i];
      del_real_chrom (individual);
      population->birthcounter++;
      individual = init_real_chrom (chromosome_size, population->birthcounter);
      do
        for (j = 0; j < chromosome_size; j++)
          individual->vector[j] = gsl_ran_flat (rng, 0, dimension);
      while (!(*is_valid) (individual));
      population->individuals[i] = individual;
    }
}

real_chrom_t **
linear_ranking (const real_pop_t *population,
                size_t total_selection)
{
  size_t i;
  real_chrom_t **individuals = calloc (population->size, sizeof (real_chrom_t *));
  real_chrom_t **selection = calloc (total_selection, sizeof (real_chrom_t *));
  for (i = 0; i < population->size; i++)
    individuals[i] = population->individuals[i];
  qsort_real_chrom (individuals, 0, population->size - 1);
  for (i = 0; i < total_selection; i++)
    selection[i] = individuals[i % 2];
  free (individuals);
  return selection;
}

real_chrom_t **
tournament_selection (const real_pop_t *population,
                      size_t tournament_size,
                      size_t total_selection)
{
  size_t i, j;
  real_chrom_t *local_best;
  real_chrom_t **selection = (real_chrom_t **) calloc (total_selection, sizeof (real_chrom_t *));
  for (i = 0; i < total_selection; i++)
    {
      real_chrom_t **tournament = (real_chrom_t **) calloc (tournament_size, sizeof (real_chrom_t *));
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

real_chrom_t **
fitness_proportional (const real_pop_t *population,
                      size_t total_selection)
{
  size_t i, j;
  double amount, roulette_value;
  double total_fitness = 0;
  real_chrom_t **selection = (real_chrom_t **) calloc (total_selection, sizeof (real_chrom_t *));
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

real_chrom_t *
real_breed (const real_chrom_t *parent_one,
            const real_chrom_t *parent_two,
            size_t birthdate,
            int (*is_valid) (real_chrom_t *))
{
  real_chrom_t *offspring = init_real_chrom (parent_one->size, birthdate);
  do
    {
      if (global_options->recombination == onepoint)
        onepoint_xover (parent_one, parent_two, offspring);
      else if (global_options->recombination == npoint)
        npoint_xover (parent_one, parent_two, offspring, NPOINTS_XOVER, NULL);
      else if (global_options->recombination == uniform)
        uniform_xover (parent_one, parent_two, offspring, HEAD_TOSSING);
      if (global_options->mutation == permutation || global_options->mutation == regenerated_permutation)
        permutation_mutation (offspring);
    }
  while (!(*is_valid) (offspring));
  return offspring;
}

void
elitist_policy_replacement (real_pop_t *population,
                            real_chrom_t **offspring)
{
  size_t i, worst_point;
  real_chrom_t *best;
  best = best_real_chrom (population);
  best = copy_real_chrom (best, best->birthdate);
  del_real_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
  worst_point = worst_real_chrom_index (population);
  population->individuals[worst_point] = best;
}

stats_t *
rkgaes (real_pop_t *population,
        size_t generations,
        double (*fitness_function) (real_chrom_t *),
        int (*is_valid) (real_chrom_t *))
{
  size_t i;
  stats_t *stats = init_stats ();
  apply_real_pop_fitness (population, fitness_function);
  reset_real_pop_stats (population, stats);
  do
    {
      real_chrom_t **selection, **offspring;
      offspring = (real_chrom_t **) calloc (population->size, sizeof (real_chrom_t *));
      if (global_options->selection == proportional)
        selection = fitness_proportional (population, population->size);
      else if (global_options->selection == tournament)
        selection = tournament_selection (population, TOURNAMENT_SIZE, population->size);
      else if (global_options->selection == ranking)
        selection = linear_ranking (population, population->size);
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[i * 2] = real_breed (selection[i * 2], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 * i + 1] = real_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
      elitist_policy_replacement (population, offspring);
      free (offspring);
      apply_real_pop_fitness (population, fitness_function);

      /* TODO Achar um melhor lugar para colocar essa mutação */
      if (global_options->mutation == regenerated || global_options->mutation == regenerated_permutation)
        random_generated_individuals (population, REGENERATED_PERCENTAGE, is_valid);
      apply_real_pop_fitness (population, fitness_function);
      /* TODO Achar um melhor lugar para colocar essa mutação */

      reset_real_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}

#endif

