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

#ifndef __evolve_hybrid_randomkey_h__
#define __evolve_hybrid_randomkey_h__

unsigned int *
integer_series (size_t dimension,
                const real_chrom_t *chrom)
{
  size_t i, j;
  size_t smallest_index, counter = 0;
  double smallest;
  double *random_key = chrom->vector;
  unsigned int *series;
  series = (unsigned int *) calloc (dimension, sizeof (unsigned int));
  for (i = 0; i < dimension; i++)
    series[i] = 0;
  for (i = 0; i < dimension; i++)
    {
      smallest_index = 0;
      smallest = random_key[0];
      for (j = 0; j < dimension; j++)
        if ((random_key[j] < smallest && !series[j])
          || series[smallest_index])
          {
            smallest = random_key[j];
            smallest_index = j;
          }
      series[smallest_index] = ++counter;
    }
  return series;
}

#endif

