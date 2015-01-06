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

#ifndef __tsp_support_h__
#define __tsp_support_h__

typedef struct {
  size_t dimension;
  unsigned long int **distances;
} salesman_t;

salesman_t *
init_salesman (size_t dimension,
               unsigned long int **distances)
{
  salesman_t *problem = (salesman_t *) malloc (sizeof (salesman_t));
  problem->dimension = dimension;
  problem->distances = distances;
  return problem;
}

void
del_salesman (salesman_t *problem)
{
  if (problem == NULL)
    return;
  size_t i;
  for (i = 0; problem->distances[i] != NULL; i++)
    free (problem->distances[i]);
  free (problem->distances);
  free (problem);
}

#endif

