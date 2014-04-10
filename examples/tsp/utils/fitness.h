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

#ifndef __tsp_fitness_h__
#define __tsp_fitness_h__

#include "support.h"

static salesman_t *local_problem;

void
set_problem (salesman_t *problem)
{
  local_problem = problem;
}

double
fitness (real_chrom_t *chrom)
{
  size_t i;
  size_t dimension = local_problem->dimension;
  long int route_length = 0;
  unsigned int *solution_route = integer_series (dimension, chrom);
  for (i = 0; i < dimension / 2; i++)
    {
      size_t from = solution_route[i * 2];
      size_t to = solution_route[i * 2 + 1];
      route_length += local_problem->distances[from][to];
    }
  free (solution_route);
  return (double) route_length;
}

#endif

