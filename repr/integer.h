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

#ifndef __evolve_repr_integer_h__
#define __evolve_repr_integer_h__

#include <stdlib.h>
#include <evolve/rng.h>

typedef struct {
  unsigned short int *vector;
  size_t size;
  size_t birthdate;
  double fitness;
} int_chrom_t;

typedef struct {
  int_chrom_t **individuals;
  size_t size;
  size_t generation;
  size_t birthcounter;
} int_pop_t;

int_chrom_t *
init_int_chrom (size_t size,
                size_t birthdate)
{
  int_chrom_t *chrom = (int_chrom_t *) malloc (sizeof (int_chrom_t));
  unsigned short int *chrom_vector = (unsigned short int *) calloc (size, sizeof (unsigned short int));
  chrom->vector = chrom_vector;
  chrom->size = size;
  chrom->birthdate = birthdate;
  return chrom;
}

void
del_int_chrom (int_chrom_t *chrom)
{
  free (chrom->vector);
  free (chrom);
}

int_chrom_t *
copy_int_chrom (const int_chrom_t *chrom,
                size_t birthdate)
{
  size_t i;
  int_chrom_t *chrom_cp = init_int_chrom (chrom->size, birthdate);
  chrom_cp->fitness = chrom->fitness;
  for (i = 0; i < chrom->size; i++)
    chrom_cp->vector[i] = chrom->vector[i];
  return chrom_cp;
}

void
apply_int_fitness (int_chrom_t *chrom,
                   double (*fitness_function) (int_chrom_t *))
{
  chrom->fitness = (*fitness_function) (chrom);
}

int_pop_t *
init_int_pop (size_t size)
{
  int_pop_t *pop = (int_pop_t *) malloc (sizeof (int_pop_t));
  int_chrom_t **individuals = (int_chrom_t **) calloc (size, sizeof (int_chrom_t *));
  pop->individuals = individuals;
  pop->size = size;
  pop->generation = 0;
  pop->birthcounter = 0;
  return pop;
}

void
del_int_pop (int_pop_t *pop)
{
  free (pop->individuals);
  free (pop);
}

void
del_int_pop_indiv (int_pop_t *pop)
{
  size_t i;
  for (i = 0; i < pop->size; i++)
    {
      int_chrom_t *individual = pop->individuals[i];
      del_int_chrom (individual);
    }
}

void
del_int_indiv (int_chrom_t **individuals,
               size_t size)
{
  size_t i;
  for (i = 0; i < size; i++)
    {
      int_chrom_t *individual = individuals[i];
      del_int_chrom (individual);
    }
}

void
random_int_pop (int_pop_t *population,
                size_t chrom_size,
                int (*is_valid) (int_chrom_t *))
{
  size_t i, j;
  for (i = 0; i < population->size; i++)
    {
      int_chrom_t *individual = init_int_chrom (chrom_size, ++population->birthcounter);
      do
        for (j = 0; j < chrom_size; j++)
          if (gsl_ran_flat (rng, 0, 100) > 50.0)
            individual->vector[j] = 0;
          else
            individual->vector[j] = 1;
      while (!(*is_valid) (individual));
      population->individuals[i] = individual;
    }
}

void
apply_int_pop_fitness (const int_pop_t *pop,
                       double (*fitness_function) (int_chrom_t *))
{
  size_t i;
  for (i = 0; i < pop->size; i++)
    {
      int_chrom_t *individual = pop->individuals[i];
      individual->fitness = (*fitness_function) (individual);
    }
}

int_chrom_t *
best_int_chrom (const int_pop_t *pop)
{
  size_t i;
  int_chrom_t *best = pop->individuals[0];
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness > best->fitness)
      best = pop->individuals[i];
  return best;
}

int_chrom_t *
worst_int_chrom (const int_pop_t *pop)
{
  size_t i;
  int_chrom_t *worst = pop->individuals[0];
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness < worst->fitness)
      worst = pop->individuals[i];
  return worst;
}

size_t
worst_int_chrom_index (const int_pop_t *pop)
{
  size_t i, index = 0;
  for (i = 1; i < pop->size; i++)
    if (pop->individuals[i]->fitness < pop->individuals[index]->fitness)
      index = i;
  return index;
}

#endif

