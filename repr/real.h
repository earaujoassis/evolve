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

#ifndef __evolve_repr_real_h__
#define __evolve_repr_real_h__

#include <stdlib.h>
#include <evolve/rng.h>

typedef struct {
  double *vector;
  size_t size;
  size_t birthdate;
  double fitness;
} real_chrom_t;

typedef struct {
  real_chrom_t **individuals;
  size_t size;
  size_t generation;
  size_t birthcounter;
} real_pop_t;

real_chrom_t *
init_real_chrom (size_t size,
                 size_t birthdate)
{
  real_chrom_t *chrom = (real_chrom_t *) malloc (sizeof (real_chrom_t));
  double *chrom_vector = (double *) calloc (size, sizeof (double));
  chrom->vector = chrom_vector;
  chrom->size = size;
  chrom->birthdate = birthdate;
  return chrom;
}

void
del_real_chrom (real_chrom_t *chrom)
{
  free (chrom->vector);
  free (chrom);
}

real_chrom_t *
copy_real_chrom (const real_chrom_t *chrom,
                 size_t birthdate)
{
  size_t i;
  real_chrom_t *chrom_cp = init_real_chrom (chrom->size, birthdate);
  chrom_cp->fitness = chrom->fitness;
  for (i = 0; i < chrom->size; i++)
    chrom_cp->vector[i] = chrom->vector[i];
  return chrom_cp;
}

void
apply_real_fitness (real_chrom_t *chrom,
                    double (*fitness_function) (real_chrom_t *))
{
  chrom->fitness = (*fitness_function) (chrom);
}

real_pop_t *
init_real_pop (size_t size)
{
  real_pop_t *pop = (real_pop_t *) malloc (sizeof (real_pop_t));
  real_chrom_t **individuals = (real_chrom_t **) calloc (size, sizeof (real_chrom_t *));
  pop->individuals = individuals;
  pop->size = size;
  pop->generation = 0;
  pop->birthcounter = 0;
  return pop;
}

void
del_real_pop (real_pop_t *pop)
{
  free (pop->individuals);
  free (pop);
}

void
del_real_pop_indiv (real_pop_t *pop)
{
  size_t i;
  for (i = 0; i < pop->size; i++)
    {
      real_chrom_t *individual = pop->individuals[i];
      del_real_chrom (individual);
    }
}

void
del_real_indiv (real_chrom_t **individuals,
                size_t size)
{
  size_t i;
  for (i = 0; i < size; i++)
    {
      real_chrom_t *individual = individuals[i];
      del_real_chrom (individual);
    }
}

void
random_real_pop (real_pop_t *population,
                 int chrom_size,
                 double a,
                 double b,
                 int (*is_valid) (real_chrom_t *))
{
  int i, j;
  for (i = 0; i < population->size; i++)
    {
      population->birthcounter++;
      real_chrom_t *individual = init_real_chrom (chrom_size, population->birthcounter);
      do
        for (j = 0; j < chrom_size; j++)
          individual->vector[j] = gsl_ran_flat (rng, a, b);
      while (!(*is_valid) (individual));
      population->individuals[i] = individual;
    }
}

void
apply_real_pop_fitness (const real_pop_t *pop,
                        double (*fitness_function) (real_chrom_t *))
{
  size_t i;
  for (i = 0; i < pop->size; i++)
    {
      real_chrom_t *individual = pop->individuals[i];
      individual->fitness = (*fitness_function) (individual);
    }
}

real_chrom_t *
best_real_chrom (const real_pop_t *pop)
{
  size_t i;
  real_chrom_t *best = pop->individuals[0];
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness > best->fitness)
      best = pop->individuals[i];
  return best;
}

real_chrom_t *
worst_real_chrom (const real_pop_t *pop)
{
  size_t i;
  real_chrom_t *worst = pop->individuals[0];
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness < worst->fitness)
      worst = pop->individuals[i];
  return worst;
}

size_t
worst_real_chrom_index (const real_pop_t *pop)
{
  size_t i, index = 0;
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness < pop->individuals[index]->fitness)
      index = i;
  return index;
}

#endif

