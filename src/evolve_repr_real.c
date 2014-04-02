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
#include <stdio.h>

#include "evolve_rng.h"
#include "evolve_repr_real.h"

evolve_real_chrom_t *
evolve_init_real_chrom (size_t size,
                        size_t birthdate)
{
  evolve_real_chrom_t *chrom = (evolve_real_chrom_t *) malloc (sizeof (evolve_real_chrom_t));
  double *chrom_vector = (double *) calloc (size, sizeof (double));
  chrom->vector = chrom_vector;
  chrom->size = size;
  chrom->birthdate = birthdate;
  return chrom;
}

void
evolve_del_real_chrom (evolve_real_chrom_t *chrom)
{
  free (chrom->vector);
  free (chrom);
}

evolve_real_chrom_t *
evolve_copy_real_chrom (const evolve_real_chrom_t *chrom,
                        size_t birthdate)
{
  size_t i;
  evolve_real_chrom_t *chrom_cp = evolve_init_real_chrom (chrom->size, birthdate);
  chrom_cp->fitness = chrom->fitness;
  for (i = 0; i < chrom->size; i++)
    chrom_cp->vector[i] = chrom->vector[i];
  return chrom_cp;
}

void
evolve_apply_real_fitness (evolve_real_chrom_t *chrom,
                           double (*fitness_function) (evolve_real_chrom_t *))
{
  chrom->fitness = (*fitness_function) (chrom);
}

evolve_real_pop_t *
evolve_init_real_pop (size_t size)
{
  evolve_real_pop_t *pop = (evolve_real_pop_t *) malloc (sizeof (evolve_real_pop_t));
  evolve_real_chrom_t **individuals = (evolve_real_chrom_t **) calloc (size, sizeof (evolve_real_chrom_t *));
  pop->individuals = individuals;
  pop->size = size;
  pop->generation = 0;
  pop->birthcounter = 0;
  return pop;
}

void
evolve_del_real_pop (evolve_real_pop_t *pop)
{
  free (pop->individuals);
  free (pop);
}

void
evolve_del_real_pop_indiv (evolve_real_pop_t *pop)
{
  size_t i;
  for (i = 0; i < pop->size; i++)
    {
      evolve_real_chrom_t *individual = pop->individuals[i];
      evolve_del_real_chrom (individual);
    }
}

void
evolve_del_real_indiv (evolve_real_chrom_t **individuals,
                       size_t size)
{
  size_t i;
  for (i = 0; i < size; i++)
    {
      evolve_real_chrom_t *individual = individuals[i];
      evolve_del_real_chrom (individual);
    }
}

void
evolve_random_real_pop (evolve_real_pop_t *population,
                 int chrom_size,
                 double a,
                 double b,
                 int (*is_valid) (evolve_real_chrom_t *))
{
  int i, j;
  for (i = 0; i < population->size; i++)
    {
      population->birthcounter++;
      evolve_real_chrom_t *individual = evolve_init_real_chrom (chrom_size, population->birthcounter);
      do
        for (j = 0; j < chrom_size; j++)
          individual->vector[j] = gsl_ran_flat (rng, a, b);
      while (!(*is_valid) (individual));
      population->individuals[i] = individual;
    }
}

void
evolve_apply_real_pop_fitness (const evolve_real_pop_t *pop,
                               double (*fitness_function) (evolve_real_chrom_t *))
{
  size_t i;
  for (i = 0; i < pop->size; i++)
    {
      evolve_real_chrom_t *individual = pop->individuals[i];
      individual->fitness = (*fitness_function) (individual);
    }
}

evolve_real_chrom_t *
evolve_best_real_chrom (const evolve_real_pop_t *pop)
{
  size_t i;
  evolve_real_chrom_t *best = pop->individuals[0];
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness > best->fitness)
      best = pop->individuals[i];
  return best;
}

evolve_real_chrom_t *
evolve_worst_real_chrom (const evolve_real_pop_t *pop)
{
  size_t i;
  evolve_real_chrom_t *worst = pop->individuals[0];
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness < worst->fitness)
      worst = pop->individuals[i];
  return worst;
}

size_t
evolve_worst_real_chrom_index (const evolve_real_pop_t *pop)
{
  size_t i, index = 0;
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness < pop->individuals[index]->fitness)
      index = i;
  return index;
}
