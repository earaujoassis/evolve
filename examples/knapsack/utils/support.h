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

#ifndef __knapsack_support_h__
#define __knapsack_support_h__

typedef struct {
  size_t items;  
  size_t compartments;
  unsigned long int *values;
  unsigned long int *capacity;
  unsigned long int **weights;
} knapsack_t;

knapsack_t *
init_knapsack (size_t items,
               size_t compartments,
               unsigned long int *values,
               unsigned long int **weights,
               unsigned long int *capacity)
{
  knapsack_t *problem;
  problem = (knapsack_t *) malloc (sizeof (knapsack_t));
  problem->items = items;
  problem->values = values;
  problem->compartments = compartments;
  problem->capacity = capacity;
  problem->weights = weights;
  return problem;
}

void
del_knapsack (knapsack_t *problem)
{
  if (problem == NULL)
    return;
  size_t i;
  for (i = 0; problem->weights[i] != NULL; i++)
    free (problem->weights[i]);
  free (problem->weights);
  free (problem->values);
  free (problem->capacity);
  free (problem);
}

#endif

