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

#ifndef __evolve_info_h__
#define __evolve_info_h__

#include <stdio.h>
#include <evolve/repr/integer.h>
#include <evolve/repr/real.h>
#include <evolve/utils/stats.h>

void
print_algo_info (const char *algo_name,
                 const char *problem_name,
                 size_t generations,
                 size_t pop_size,
                 unsigned long int seed)
{
  printf ("Algorithm name: %s\n", algo_name);
  printf ("Problem (optimization objective): %s\n", problem_name);
  printf ("Simulation time limit (# generations): %ld\n", generations);
  printf ("Random number seed (positive integer): %ld\n", seed);
  printf ("Population size: %d\n", pop_size);
}

static void
print_common_info (const stats_t *global_stats,
                   const stats_t *local_stats)
{
  if (global_stats != NULL)
    printf ("Global fitness:\tbest = %.15lf,\tmean = %.15lf,\tworst = %.15lf\n", \
      global_stats->max_fitness, global_stats->ave_fitness, global_stats->min_fitness);
  if (local_stats != NULL)
    printf ("Local fitness:\tbest = %.15lf,\tmean = %.15lf,\tworst = %.15lf\n", \
      local_stats->max_fitness, local_stats->ave_fitness, local_stats->min_fitness);
}

void
print_real_pop (const real_pop_t *population,
                const stats_t *global_stats)
{
  size_t i, j;
  stats_t *local_stats = init_stats ();
  reset_real_pop_stats (population, local_stats);
  print_common_info (global_stats, local_stats);
  printf ("Population data after %ld births (generation %d):\n", population->birthcounter, population->generation);
  printf ("Indiv \t bithdate \t fitness \t\t\t gene values\n");
  for (i = 0; i < population->size; i++)
    {
      real_chrom_t *individual = population->individuals[i];
      printf ("%3d \t %8d \t %.15lf \t\t", i + 1, individual->birthdate, individual->fitness);
      for (j = 0; j < individual->size; j++)
        printf ("%.15lf  ", individual->vector[j]);
      printf ("\n");
    }
  printf ("\n");
  del_stats (local_stats);
}

void
print_int_pop (const int_pop_t *population,
               const stats_t *global_stats)
{
  size_t i, j;
  stats_t *local_stats = init_stats ();
  reset_int_pop_stats (population, local_stats);
  print_common_info (global_stats, local_stats);
  printf ("Population data after %ld births (generation %d):\n", population->birthcounter, population->generation);
  printf ("Indiv \t bithdate \t fitness \t\t\t gene values\n");
  for (i = 0; i < population->size; i++)
    {
      int_chrom_t *individual = population->individuals[i];
      printf ("%3d \t %8d \t %.15lf \t\t", i + 1, individual->birthdate, individual->fitness);
      for (j = 0; j < individual->size; j++)
        printf ("%1d", individual->vector[j]);
      printf ("\n");
    }
  printf ("\n");
  del_stats (local_stats);
}

void
print_real_chrom (const real_chrom_t *individual)
{
  size_t i;
  printf ("Chromosome: ");
  for (i = 0; i < individual->size; i++)
    printf ("%1.0lf ", individual->vector[i]);
  printf ("\n");
}

#endif

