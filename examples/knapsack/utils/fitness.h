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

#ifndef __knapsack_fitness_h__
#define __knapsack_fitness_h__

#include "support.h"

static knapsack_t *local_problem;

void
set_problem (knapsack_t *problem)
{
  local_problem = problem;
}

int
is_subjected_original (int_chrom_t *chrom)
{
  size_t compartment, item;
  unsigned long int total_weight;
  for (compartment = 0; compartment < local_problem->compartments; compartment++)
    {
      total_weight = 0;
      for (item = 0; item < local_problem->items; item++)
        if (chrom->vector[item])
          total_weight += local_problem->weights[compartment][item];
      if (total_weight > local_problem->capacity[compartment])
        return 0;
    }
  return 1;
}

int
is_subjected (int_chrom_t *chrom)
{
  size_t item, compartment;
  size_t compartments = local_problem->compartments;
  size_t items = local_problem->items;
  unsigned long int *weight_per_compartment;
  weight_per_compartment = (unsigned long int *) calloc (compartments, sizeof (unsigned long int));
  for (compartment = 0; compartment < compartments; compartment++)
    {
      weight_per_compartment[compartment] = 0;
      for (item = 0; item < items; item++)
        if (chrom->vector[item])
          weight_per_compartment[compartment] += local_problem->weights[compartment][item];
    }
  for (compartment = 0; compartment < compartments; compartment++)
    if (weight_per_compartment[compartment] > local_problem->capacity[compartment])
      {
        free (weight_per_compartment);
        return 0;
      }
  free (weight_per_compartment);
  return 1;
}

void
mutate (int_chrom_t *chrom)
{
  size_t point = gsl_rng_uniform_int (rng, chrom->size - 1);
  while (!chrom->vector[point]) /* chrom->vector[point] != 1 */
    point = gsl_rng_uniform_int (rng, chrom->size - 1);
  chrom->vector[point] = 0;
}

double
fitness (int_chrom_t *chrom)
{
  size_t i;
  unsigned long int fitness = 0;
  for (i = 0; i < local_problem->items; i++)
    if (chrom->vector[i])
      fitness += local_problem->values[i];
  return (double) fitness;
}

int
check (int_chrom_t *chrom)
{
  while (!is_subjected_original (chrom))
    mutate (chrom);
  return 1;
}

#endif

