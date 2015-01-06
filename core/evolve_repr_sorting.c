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

#include "evolve_repr_integer.h"
#include "evolve_repr_real.h"
#include "evolve_repr_sorting.h"

long int
evolve_qsort_int_partition (evolve_int_chrom_t **individuals,
                            long int begin,
                            long int end)
{
  long int i = begin + 1;
  long int j = end;
  evolve_int_chrom_t *pivot = individuals[begin];
  evolve_int_chrom_t *aux;
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
evolve_qsort_int_chrom (evolve_int_chrom_t **individuals,
                        long int begin,
                        long int end)
{
  long int pivot;
  if (end > begin)
    {
      pivot = evolve_qsort_int_partition (individuals, begin, end);
      evolve_qsort_int_chrom (individuals, begin, pivot - 1);
      evolve_qsort_int_chrom (individuals, pivot + 1, end);
    }
}

long int
evolve_qsort_real_partition (evolve_real_chrom_t **individuals,
                             long int begin,
                             long int end)
{
  long int i = begin + 1;
  long int j = end;
  evolve_real_chrom_t *pivot = individuals[begin];
  evolve_real_chrom_t *aux;
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
evolve_qsort_real_chrom (evolve_real_chrom_t **individuals,
                         long int begin,
                         long int end)
{
  long int pivot;
  if (end > begin)
    {
      pivot = evolve_qsort_real_partition (individuals, begin, end);
      evolve_qsort_real_chrom (individuals, begin, pivot - 1);
      evolve_qsort_real_chrom (individuals, pivot + 1, end);
    }
}
