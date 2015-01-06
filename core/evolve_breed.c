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
#include "evolve_int_mut.h"
#include "evolve_int_rec.h"
#include "evolve_real_mut.h"
#include "evolve_real_rec.h"
#include "evolve_o_rkgaes.h"
#include "evolve_breed.h"

evolve_int_chrom_t *
evolve_int_breed (const evolve_int_chrom_t *parent_one,
                  const evolve_int_chrom_t *parent_two,
                  size_t birthdate,
                  int (*is_valid) (evolve_int_chrom_t *))
{
  evolve_int_chrom_t *offspring = evolve_init_int_chrom (parent_one->size, birthdate);
  do
    {
      if (gsl_ran_flat (rng, 0, 100) < XOVER_RATE)
#ifdef XOVER_POINTS
        evolve_int_npoint_xover (parent_one, parent_two, offspring, XOVER_POINTS);
#else
        evolve_int_onepoint_xover (parent_one, parent_two, offspring);
#endif
      evolve_per_int_mutation (offspring, MUTATION_RATE);
    }
  while (!(*is_valid) (offspring));
  return offspring;
}

evolve_real_chrom_t *
evolve_real_breed (const evolve_real_chrom_t *parent_one,
                   const evolve_real_chrom_t *parent_two,
                   size_t birthdate,
                   int (*is_valid) (evolve_real_chrom_t *))
{
  evolve_real_chrom_t *offspring = evolve_init_real_chrom (parent_one->size, birthdate);
  do
    {
      if (global_options->recombination == onepoint)
        evolve_real_onepoint_xover (parent_one, parent_two, offspring);
      else if (global_options->recombination == npoint)
        evolve_real_npoint_xover (parent_one, parent_two, offspring, NPOINTS_XOVER, NULL);
      else if (global_options->recombination == uniform)
        evolve_real_uniform_xover (parent_one, parent_two, offspring, HEAD_TOSSING);
      if (global_options->mutation == permutation || global_options->mutation == regenerated_permutation)
        evolve_real_permutation_mutation (offspring);
    }
  while (!(*is_valid) (offspring));
  return offspring;
}
