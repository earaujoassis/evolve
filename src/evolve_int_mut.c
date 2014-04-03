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

#include "evolve_rng.h"
#include "evolve_int_mut.h"

void
evolve_per_int_mutation (evolve_int_chrom_t *chrom,
                         double mutation_chance)
{
  size_t i;
  for (i = 0; i < chrom->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < mutation_chance)
      chrom->vector[i] = chrom->vector[i] ? 0 : 1;
}
