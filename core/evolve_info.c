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

#include <stdio.h>

#include "evolve_info.h"

void
evolve_print_algo_info (const char *algo_name,
                        const char *problem_name,
                        size_t generations,
                        size_t pop_size,
                        unsigned long int seed)
{
  printf ("%s\n", algo_name);
  printf ("  Problem (optimization objective): %s\n", problem_name);
  printf ("  Simulation time limit (# generations): %ld\n", generations);
  printf ("  Random number seed (positive integer): %ld\n", seed);
  printf ("  Population size: %d\n", pop_size);
}

void
evolve_print_common_info (const evolve_stats_t *global_stats,
                          const evolve_stats_t *local_stats)
{
  if (global_stats != NULL)
    printf ("  Global fitness:\tbest = %.15lf,\tmean = %.15lf,\tworst = %.15lf\n", \
      global_stats->max_fitness, global_stats->ave_fitness, global_stats->min_fitness);
  if (local_stats != NULL)
    printf ("  Local fitness:\tbest = %.15lf,\tmean = %.15lf,\tworst = %.15lf\n", \
      local_stats->max_fitness, local_stats->ave_fitness, local_stats->min_fitness);
}

void
evolve_print_real_pop (const evolve_real_pop_t *population,
                       const evolve_stats_t *global_stats)
{
  size_t i, j;
  evolve_stats_t *local_stats = evolve_init_stats ();
  evolve_reset_real_pop_stats (population, local_stats);
  evolve_print_common_info (global_stats, local_stats);
  printf ("  Population data after %ld births (generation %d):\n", population->birthcounter, population->generation);
  printf ("  Indiv \t bithdate \t fitness \t\t\t gene values\n");
  for (i = 0; i < population->size; i++)
    {
      evolve_real_chrom_t *individual = population->individuals[i];
      printf ("\t %3d \t %8d \t %.5lf \t\t", i + 1, individual->birthdate, individual->fitness);
      if (individual->size > GENES_OUTPUT_LIMIT)
        {
          for (j = 0; j < GENES_OUTPUT_LIMIT; j++)
            printf ("%.15lf  ", individual->vector[j]);
          printf("...");
        }
      else
        {
          for (j = 0; j < individual->size; j++)
            printf ("%.15lf  ", individual->vector[j]);
        }
      printf ("\n");
    }
  printf ("\n");
  evolve_del_stats (local_stats);
}

void
evolve_print_int_pop (const evolve_int_pop_t *population,
                      const evolve_stats_t *global_stats)
{
  size_t i, j;
  evolve_stats_t *local_stats = evolve_init_stats ();
  evolve_reset_int_pop_stats (population, local_stats);
  evolve_print_common_info (global_stats, local_stats);
  printf ("  Population data after %ld births (generation %d):\n", population->birthcounter, population->generation);
  printf ("  Indiv \t bithdate \t fitness \t\t gene values\n");
  for (i = 0; i < population->size; i++)
    {
      evolve_int_chrom_t *individual = population->individuals[i];
      printf ("   %3d \t       %8d \t   %5.5lf    \t\t", i + 1, individual->birthdate, individual->fitness);
      if (individual->size > GENES_OUTPUT_LIMIT)
        {
          for (j = 0; j < GENES_OUTPUT_LIMIT; j++)
            printf ("%1d", individual->vector[j]);
          printf("...");
        }
      else
        {
          for (j = 0; j < individual->size; j++)
            printf ("%1d", individual->vector[j]);
        }
      printf ("\n");
    }
  printf ("\n");
  evolve_del_stats (local_stats);
}

void
evolve_print_real_chrom (const evolve_real_chrom_t *individual)
{
  size_t i;
  printf ("  Chromosome: ");
  for (i = 0; i < individual->size; i++)
    printf ("%1.0lf ", individual->vector[i]);
  printf ("\n");
}
