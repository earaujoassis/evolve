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

#ifndef __evolve_ga_canonical_h__
#define __evolve_ga_canonical_h__

#include <evolve/paradigms/common.h>

#ifndef XOVER_RATE
#define XOVER_RATE         70.0
#endif
#ifndef MUTATION_RATE
#define MUTATION_RATE      0.20
#endif

void
uniform_xover (const int_chrom_t *parent_one,
               const int_chrom_t *parent_two,
               int_chrom_t *offspring,
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
onepoint_xover (const int_chrom_t *parent_one,
                const int_chrom_t *parent_two,
                int_chrom_t *offspring)
{
  size_t i;
  size_t point = gsl_rng_uniform_int (rng, offspring->size - 1);
  for (i = 0; i < point; i++)
    offspring->vector[i] = parent_one->vector[i];
  for (i = point; i < offspring->size; i++)
    offspring->vector[i] = parent_two->vector[i];
}

void
npoint_xover (const int_chrom_t *parent_one,
              const int_chrom_t *parent_two,
              int_chrom_t *offspring,
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
per_int_mutation (int_chrom_t *chrom,
                  double mutation_chance)
{
  size_t i;
  for (i = 0; i < chrom->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < mutation_chance)
      chrom->vector[i] = chrom->vector[i] ? 0 : 1;
}

int_chrom_t *
int_breed (const int_chrom_t *parent_one,
           const int_chrom_t *parent_two,
           size_t birthdate,
           int (*is_valid) (int_chrom_t *))
{
  int_chrom_t *offspring = init_int_chrom (parent_one->size, birthdate);
  do
    {
      if (gsl_ran_flat (rng, 0, 100) < XOVER_RATE)
#ifdef XOVER_POINTS
        npoint_xover (parent_one, parent_two, offspring, XOVER_POINTS);
#else
        onepoint_xover (parent_one, parent_two, offspring);
#endif
      per_int_mutation (offspring, MUTATION_RATE);
    }
  while (!(*is_valid) (offspring));
  return offspring;
}

int_chrom_t **
tournament_selection (const int_pop_t *population,
                      size_t tournament_size,
                      size_t total_selection)
{
  size_t i, j;
  int_chrom_t *local_best;
  int_chrom_t **selection = (int_chrom_t **) calloc (total_selection, sizeof (int_chrom_t *));
  for (i = 0; i < total_selection; i++)
    {
      int_chrom_t **tournament = (int_chrom_t **) calloc (tournament_size, sizeof (int_chrom_t *));
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

int_chrom_t **
roulette_wheel_selection (const int_pop_t *population,
                          size_t total_selection)
{
  size_t i, j;
  double amount, roulette_value;
  double total_fitness = 0;
  int_chrom_t **selection = (int_chrom_t **) calloc (total_selection, sizeof (int_chrom_t *));
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
generational_replacement (int_pop_t *population,
                          int_chrom_t **offspring)
{
  size_t i;
  del_int_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
}

void
elitist_policy_replacement (int_pop_t *population,
                            int_chrom_t **offspring)
{
  size_t i, worst_point;
  int_chrom_t *best;
  best = best_int_chrom (population);
  best = copy_int_chrom (best, best->birthdate);
  del_int_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
  worst_point = worst_int_chrom_index (population);
  population->individuals[worst_point] = best;
}

stats_t *
int_ga (int_pop_t *population,
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
          offspring[2 * i + 1] = int_breed (selection[2 * i + 1], selection[2 * i], \
            ++population->birthcounter, is_valid);
        }
      free (selection);
#ifdef ELITIST_POLICY
      elitist_policy_replacement (population, offspring);
#else
      generational_replacement (population, offspring);
#endif
      free (offspring);
      apply_int_pop_fitness (population, fitness_function);
      reset_int_pop_stats (population, stats);
      population->generation++;
    }
  while (population->generation < generations);
  return stats;
}

#endif

