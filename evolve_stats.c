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

evolve_stats_t *
evolve_init_stats ()
{
  evolve_stats_t *stats = (evolve_stats_t *) malloc (sizeof (evolve_stats_t));
  stats->first_time = 1;
  return stats;
}

void
evolve_del_stats (evolve_stats_t *stats)
{
  free (stats);
}

void
evolve_reset_real_pop_stats (const evolve_real_pop_t *pop,
                             evolve_stats_t *stats)
{
  size_t i;
  double max, ave = 0, min;
  for (i = 0; i < pop->size; i++)
    {
      if (!i)
        {
          max = pop->individuals[i]->fitness;
          min = pop->individuals[i]->fitness;
        }
      if (pop->individuals[i]->fitness > max)
        max = pop->individuals[i]->fitness;
      if (pop->individuals[i]->fitness < min)
        min = pop->individuals[i]->fitness;
      ave += pop->individuals[i]->fitness;
    }
  ave = ave / pop->size;
  if (stats->first_time)
    {
      stats->first_time = 0;
      stats->ave_fitness = ave;
      stats->max_fitness = max;
      stats->min_fitness = min;
      return;
    }
  if (max > stats->max_fitness)
    stats->max_fitness = max;
  if (min < stats->min_fitness)
    stats->min_fitness = min;
  stats->ave_fitness = (stats->ave_fitness + ave) / 2;
}

void
evolve_reset_int_pop_stats (const evolve_int_pop_t *pop,
                            evolve_stats_t *stats)
{
  size_t i;
  double max, ave = 0, min;
  for (i = 0; i < pop->size; i++)
    {
      if (!i)
        {
          max = pop->individuals[i]->fitness;
          min = pop->individuals[i]->fitness;
        }
      if (pop->individuals[i]->fitness > max)
        max = pop->individuals[i]->fitness;
      if (pop->individuals[i]->fitness < min)
        min = pop->individuals[i]->fitness;
      ave += pop->individuals[i]->fitness;
    }
  ave = ave / pop->size;
  if (stats->first_time)
    {
      stats->first_time = 0;
      stats->ave_fitness = ave;
      stats->max_fitness = max;
      stats->min_fitness = min;
      return;
    }
  if (max > stats->max_fitness)
    stats->max_fitness = max;
  if (min < stats->min_fitness)
    stats->min_fitness = min;
  stats->ave_fitness = (stats->ave_fitness + ave) / 2;
}
