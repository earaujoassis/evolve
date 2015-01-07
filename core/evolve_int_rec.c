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

#include "evolve_rng.h"
#include "evolve_int_rec.h"

void
evolve_int_uniform_xover (const evolve_int_chrom_t *parent_one,
                          const evolve_int_chrom_t *parent_two,
                          evolve_int_chrom_t *offspring,
                          double parameterized)
{
  double rand;
  size_t i;
  for (i = 0; i < offspring->size; i++)
    {
      rand = gsl_ran_flat (rng, 0, 100);
      if (rand < parameterized) /* Head */
        offspring->vector[i] = parent_one->vector[i];
      else /* Tails */
        offspring->vector[i] = parent_two->vector[i];
    }
}

void
evolve_int_onepoint_xover (const evolve_int_chrom_t *parent_one,
                           const evolve_int_chrom_t *parent_two,
                           evolve_int_chrom_t *offspring)
{
  size_t i;
  size_t point = gsl_rng_uniform_int (rng, offspring->size - 1);
  for (i = 0; i < point; i++)
    offspring->vector[i] = parent_one->vector[i];
  for (i = point; i < offspring->size; i++)
    offspring->vector[i] = parent_two->vector[i];
}

void
evolve_int_npoint_xover (const evolve_int_chrom_t *parent_one,
                         const evolve_int_chrom_t *parent_two,
                         evolve_int_chrom_t *offspring,
                         size_t num_points)
{
  size_t i, point, deslocation;
  size_t *points = (size_t *) calloc (num_points + 1, sizeof (size_t));
  points[num_points] = parent_one->size;
  for (i = 0; i < num_points; i++)
    {
      size_t begin = i * (parent_one->size / (num_points + 1));
      size_t end = (i + 1) * (parent_one->size / (num_points + 1));
      points[i] = gsl_rng_uniform_int (rng, end - begin) + begin;
    }
  deslocation = 0;
  for (point = 0; point < num_points + 1; point++)
    {
      for (i = deslocation; i < points[point]; i++)
        if (point % 2)
          offspring->vector[i] = parent_one->vector[i];
        else
          offspring->vector[i] = parent_two->vector[i];
      deslocation = points[point];
    }
  free (points);
}
