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

#ifndef __evolve_permutation_h__
#define __evolve_permutation_h__

#define PROB_EXCHANGE           35.0
#define PROB_INSERTION          35.0
#define PROB_INVERSION          10.0
#define PROB_SHIFTING           20.0

void
inversion_operator (real_chrom_t *chrom)
{
  size_t i, j;
  double aux;
  double *vector = chrom->vector;
  i = gsl_rng_uniform_int (rng, chrom->size - 2);
  j = gsl_rng_uniform_int (rng, chrom->size - i) + i;
  for (; i < j; i++, j--)
    {
      aux = vector[i];
      vector[i] = vector[j];
      vector[j] = aux;
    }
}

void
insertion_operator (real_chrom_t *chrom)
{
  size_t i, source, destination;
  double element, *vector = chrom->vector;
  source = gsl_rng_uniform_int (rng, chrom->size);
  do
    destination = gsl_rng_uniform_int (rng, chrom->size);
  while (source == destination);
  element = vector[source];
  if (destination > source)
    {
      for (i = source; i < destination; i++)
        vector[i] = vector[i + 1];
      vector[destination] = element;
    }
  else
    {
      for (i = source; i > destination; i--)
        vector[i] = vector[i - 1];
      vector[destination] = element;
    }
}

void
exchange_operator (real_chrom_t *chrom) /* Swapping */
{
  double aux, *vector = chrom->vector;
  size_t source, destination;
  source = gsl_rng_uniform_int (rng, chrom->size);
  do
    destination = gsl_rng_uniform_int (rng, chrom->size);
  while (source == destination);
  aux = vector[source];
  vector[source] = vector[destination];
  vector[destination] = aux;
}

void
shifting_operator (real_chrom_t *chrom) /* Long distance movement */
{
  size_t i, j;
  size_t begin, end, length, destination;
  double aux, *vector = chrom->vector;
  begin = gsl_rng_uniform_int (rng, chrom->size - 2);
  end = gsl_rng_uniform_int (rng, chrom->size - begin) + begin;
  destination = gsl_rng_uniform_int (rng, chrom->size - end) + end;
  length = destination - end;
  for (i = 0; i < length; i++)
    {
      aux = vector[end + 1];
      for (j = end; j > begin - 1; j--)
        vector[j + 1] = vector[j];
      vector[j + 1] = aux;
      begin++;
      end++;
    }
}

#endif

