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

#ifndef __knapsack_parser_h__
#define __knapsack_parser_h__

#include "support.h"

knapsack_t *
init_instance (FILE *stream)
{
  unsigned int i, j, k;
  unsigned int items, compartments;
  unsigned long int *values, **weights, *capacity;
  knapsack_t *problem;

  items = 0;
  compartments = 0;
  fscanf (stream, "%d %d %*d", &items, &compartments);
  if (!items || !compartments)
    return NULL;
  values = (unsigned long int *) calloc (items, sizeof (unsigned long int));
  for (i = 0; i < items; i++)
    fscanf(stream, " %ld ", &values[i]);
  weights = (unsigned long int **) calloc (compartments, 1 + sizeof (unsigned long int *));
  weights[compartments] = NULL;
  for (i = 0; i < compartments; i++)
    weights[i] = (unsigned long int *) calloc (items, sizeof (unsigned long int));
  for (i = 0; i < compartments; i++)
    for (j = 0; j < items; j++)
      fscanf (stream, " %ld ", &weights[i][j]);
  capacity = (unsigned long int *) calloc (compartments, sizeof (unsigned long int));
  for (i = 0; i < compartments; i++)
    fscanf (stream, " %ld ", &capacity[i]);
  problem = init_knapsack (items, compartments, values, weights, capacity);
  return problem;
}

#endif

