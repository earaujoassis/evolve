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
#include "evolve_repr_integer.h"
#include "evolve_stats.h"
#include "evolve_ga.h"

void
evolve_int_uniform_xover (const evolve_int_chrom_t *parent_one,
                          const evolve_int_chrom_t *parent_two,
                          evolve_int_chrom_t *offspring,
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
evolve_int_onepoint_xover (const evolve_int_chrom_t *parent_one,
                           const evolve_int_chrom_t *parent_two,
                           evolve_int_chrom_t *offspring)
{
  size_t i;
  size_t point = gsl_rng_uniform_int (rng, offspring->size - 1);
  for (i = 0; i < point; i++)
    offspring->vector[i] = parent_one->vector[i];
  for (i = point; i < offspring->size; i++)
    offspring->vector[i] = parent_two->vector[i];
}

void
evolve_int_npoint_xover (const evolve_int_chrom_t *parent_one,
                         const evolve_int_chrom_t *parent_two,
                         evolve_int_chrom_t *offspring,
                         size_t num_points)
{
  size_t i, point, deslocation;
  size_t *points = (size_t *) calloc (num_points + 1, sizeof (size_t));
  points[num_points] = parent_one->size;
  for (i = 0; i < num_points; i++)
    {
      size_t begin = i * (parent_one->size / (num_points + 1));
      size_t end = (i + 1) * (parent_one->size / (num_points + 1));
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
evolve_per_int_mutation (evolve_int_chrom_t *chrom,
                         double mutation_chance)
{
  size_t i;
  for (i = 0; i < chrom->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < mutation_chance)
      chrom->vector[i] = chrom->vector[i] ? 0 : 1;
}

evolve_int_chrom_t *
evolve_int_breed (const evolve_int_chrom_t *parent_one,
                  const evolve_int_chrom_t *parent_two,
                  size_t birthdate,
                  int (*is_valid) (evolve_int_chrom_t *))
{
  evolve_int_chrom_t *offspring = evolve_init_int_chrom (parent_one->size, birthdate);
  do
    {
      if (gsl_ran_flat (rng, 0, 100) < XOVER_RATE)
#ifdef XOVER_POINTS
        evolve_int_npoint_xover (parent_one, parent_two, offspring, XOVER_POINTS);
#else
        evolve_int_onepoint_xover (parent_one, parent_two, offspring);
#endif
      evolve_per_int_mutation (offspring, MUTATION_RATE);
    }
  while (!(*is_valid) (offspring));
  return offspring;
}

evolve_int_chrom_t **
evolve_int_tournament_selection (const evolve_int_pop_t *population,
                                 size_t tournament_size,
                                 size_t total_selection)
{
  size_t i, j;
  evolve_int_chrom_t *local_best;
  evolve_int_chrom_t **selection = (evolve_int_chrom_t **) calloc (total_selection, sizeof (evolve_int_chrom_t *));
  for (i = 0; i < total_selection; i++)
    {
      evolve_int_chrom_t **tournament = (evolve_int_chrom_t **) calloc (tournament_size, sizeof (evolve_int_chrom_t *));
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

evolve_int_chrom_t **
evolve_roulette_wheel_selection (const evolve_int_pop_t *population,
                                 size_t total_selection)
{
  size_t i, j;
  double amount, roulette_value;
  double total_fitness = 0;
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
evolve_generational_replacement (evolve_int_pop_t *population,
                                 evolve_int_chrom_t **offspring)
{
  size_t i;
  evolve_del_int_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
}

void
elitist_policy_replacement (evolve_int_pop_t *population,
                            evolve_int_chrom_t **offspring)
{
  size_t i, worst_point;
  evolve_int_chrom_t *best;
  best = evolve_best_int_chrom (population);
  best = evolve_copy_int_chrom (best, best->birthdate);
  evolve_del_int_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
  worst_point = evolve_worst_int_chrom_index (population);
  population->individuals[worst_point] = best;
}

evolve_stats_t *
evolve_int_ga (evolve_int_pop_t *population,
               size_t generations,
               double (*fitness_function) (evolve_int_chrom_t *),
               int (*is_valid) (evolve_int_chrom_t *))
{
  size_t i;
  evolve_stats_t *stats = evolve_init_stats ();
  evolve_apply_int_pop_fitness (population, fitness_function);
  evolve_reset_int_pop_stats (population, stats);
  do
    {
      evolve_int_chrom_t **selection, **offspring;
      offspring = (evolve_int_chrom_t **) calloc (population->size, sizeof (evolve_int_chrom_t *));
#ifdef TOURNAMENT_SIZE
      selection = evolve_int_tournament_selection (population, TOURNAMENT_SIZE, population->size);
#else
      selection = evolve_roulette_wheel_selection (population, population->size);
#endif
      for (i = 0; i < population->size / 2; i++)
        {
          offspring[2 * i] = evolve_int_breed (selection[2 * i], selection[2 * i + 1], \
            ++population->birthcounter, is_valid);
          offspring[2 * i + 1] = evolve_int_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
#ifdef ELITIST_POLICY
      elitist_policy_replacement (population, offspring);
#else
      evolve_generational_replacement (population, offspring);
#endif
      free (offspring);
      evolve_apply_int_pop_fitness (population, fitness_function);
      evolve_reset_int_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}
