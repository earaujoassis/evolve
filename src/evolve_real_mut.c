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

#include <math.h>

#include "evolve_rng.h"
#include "evolve_real_mut.h"

void
evolve_real_delta_mutation (evolve_real_chrom_t *chrom,
                            double step_size)
{
  int i;
  for (i = 0; i < chrom->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < MUTATION_RATE)
      if (gsl_ran_flat (rng, 0, 100) < 50.0)
        chrom->vector[i] += gsl_ran_flat (rng, 0, step_size);
      else
        chrom->vector[i] -= gsl_ran_flat (rng, 0, step_size);
}

void
evolve_real_gaussian_mutation (evolve_real_chrom_t *chrom,
                               double step_size)
{
  int i;
  double sigma = step_size / sqrt (2 / M_PI);
  for (i = 0; i < chrom->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < MUTATION_RATE)
      chrom->vector[i] += gsl_ran_gaussian (rng, sigma);
}

void
evolve_real_permutation_mutation (evolve_real_chrom_t *offspring)
{
  double rand = gsl_ran_flat (rng, 0, 100);
  if (rand <= PROB_EXCHANGE)
    evolve_exchange_operator (offspring);
  else if (PROB_EXCHANGE < rand && rand <= PROB_EXCHANGE + PROB_INSERTION)
    evolve_insertion_operator (offspring);
  else if (PROB_EXCHANGE + PROB_INSERTION < rand
    && rand < PROB_EXCHANGE + PROB_INSERTION + PROB_INVERSION)
    evolve_inversion_operator (offspring);
  else
    evolve_shifting_operator (offspring);
}
