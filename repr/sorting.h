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

#ifndef __evolve_sorting_h__
#define __evolve_sorting_h__

#include <evolve/repr/integer.h>
#include <evolve/repr/real.h>

long int
qsort_int_partition (int_chrom_t **individuals,
                     long int begin,
                     long int end)
{
  long int i = begin + 1;
  long int j = end;
  int_chrom_t *pivot = individuals[begin];
  int_chrom_t *aux;
  while (i <= j)
    {
      if (individuals[i]->fitness > pivot->fitness)
        i++;
      else if (pivot->fitness > individuals[j]->fitness)
        j--;
      else
        {
          aux = individuals[i];
          individuals[i] = individuals[j];
          individuals[j] = aux;
          i++;
          j--;
        }
    }
  individuals[begin] = individuals[j];
  individuals[j] = pivot;
  return j;
}

void
qsort_int_chrom (int_chrom_t **individuals,
                 long int begin,
                 long int end)
{
  long int pivot;
  if (end > begin)
    {
      pivot = qsort_int_partition (individuals, begin, end);
      qsort_int_chrom (individuals, begin, pivot - 1);
      qsort_int_chrom (individuals, pivot + 1, end);
    }
}

long int
qsort_real_partition (real_chrom_t **individuals,
                      long int begin,
                      long int end)
{
  long int i = begin + 1;
  long int j = end;
  real_chrom_t *pivot = individuals[begin];
  real_chrom_t *aux;
  while (i <= j)
    {
      if (individuals[i]->fitness > pivot->fitness)
        i++;
      else if (pivot->fitness > individuals[j]->fitness)
        j--;
      else
        {
          aux = individuals[i];
          individuals[i] = individuals[j];
          individuals[j] = aux;
          i++;
          j--;
        }
    }
  individuals[begin] = individuals[j];
  individuals[j] = pivot;
  return j;
}

void
qsort_real_chrom (real_chrom_t **individuals,
                  long int begin,
                  long int end)
{
  long int pivot;
  if (end > begin)
    {
      pivot = qsort_real_partition (individuals, begin, end);
      qsort_real_chrom (individuals, begin, pivot - 1);
      qsort_real_chrom (individuals, pivot + 1, end);
    }
}

#endif

