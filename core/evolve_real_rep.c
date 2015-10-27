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

#include "evolve_real_rep.h"

void
evolve_real_elitist_policy_replacement (evolve_real_pop_t *population,
                                        evolve_real_chrom_t **offspring)
{
  size_t i, worst_point;
  evolve_real_chrom_t *best;
  best = evolve_best_real_chrom (population);
  best = evolve_copy_real_chrom (best, best->birthdate);
  evolve_del_real_pop_indiv (population);
  for (i = 0; i < population->size; i++)
    population->individuals[i] = offspring[i];
  worst_point = evolve_worst_real_chrom_index (population);
  population->individuals[worst_point] = best;
}