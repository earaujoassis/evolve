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
#include "evolve_stats.h"
#include "evolve_real_mut.h"
#include "evolve_ev.h"

evolve_real_chrom_t *
evolve_real_breed_ev (const evolve_real_chrom_t *parent,
                      int birthdate,
                      double step_size,
                      void (*mutation_function) (evolve_real_chrom_t *, double),
                      int (*is_valid) (evolve_real_chrom_t *))
{
  evolve_real_chrom_t *offspring;
  offspring = evolve_copy_real_chrom (parent, birthdate);
  (*mutation_function) (offspring, step_size);
  if (!(*is_valid) (offspring))
    {
      evolve_del_real_chrom (offspring);
      offspring = evolve_copy_real_chrom (parent, birthdate);
    }
  return offspring;
}

void
evolve_real_ev (evolve_real_pop_t *population,
                long generations,
                double step_size,
                void (*mutation_function) (evolve_real_chrom_t *, double),
                double (*fitness_function) (evolve_real_chrom_t *),
                int (*is_valid) (evolve_real_chrom_t *))
{
  evolve_stats_t *stats = evolve_init_stats ();
  evolve_apply_real_pop_fitness (population, fitness_function);
  evolve_reset_real_pop_stats (population, stats);
  do
    {
      int random_individual;
      evolve_real_chrom_t *parent, *offspring, *victim;
      long pop_size = population->size;

      random_individual = gsl_rng_uniform_int (rng, pop_size);
      parent = population->individuals[random_individual];
      population->birthcounter++;
      offspring = evolve_real_breed_ev (parent, population->birthcounter, step_size, mutation_function, is_valid);
      offspring->fitness = (*fitness_function) (offspring);
      random_individual = gsl_rng_uniform_int (rng, pop_size);
      victim = population->individuals[random_individual];
      if (offspring->fitness > victim->fitness)
        {
          population->individuals[random_individual] = offspring;
          evolve_del_real_chrom (victim);
        }
      else
        evolve_del_real_chrom (offspring);
      population->generation++;
      evolve_reset_real_pop_stats (population, stats);
    }
  while (population->generation < generations);
  evolve_print_real_pop (population, stats);
  evolve_del_stats (stats);
}
