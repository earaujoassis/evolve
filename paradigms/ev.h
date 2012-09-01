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

#ifndef __evolve_ev_h__
#define __evolve_ev_h__

#include <math.h>
#include <evolve/paradigms/common.h>

#define DEFAULT_MUTATION_RATE      0.150

void
real_delta_mutation (real_chrom_t *chrom,
                     double step_size)
{
  int i;
  for (i = 0; i < chrom->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < DEFAULT_MUTATION_RATE)
      if (gsl_ran_flat (rng, 0, 100) < 50.0)
        chrom->vector[i] += gsl_ran_flat (rng, 0, step_size);
      else
        chrom->vector[i] -= gsl_ran_flat (rng, 0, step_size);
}

void
real_gaussian_mutation (real_chrom_t *chrom,
                        double step_size)
{
  int i;
  double sigma = step_size / sqrt (2 / M_PI);
  for (i = 0; i < chrom->size; i++)
    if (gsl_ran_flat (rng, 0, 100) < DEFAULT_MUTATION_RATE)
      chrom->vector[i] += gsl_ran_gaussian (rng, sigma);
}

real_chrom_t *
real_breed (const real_chrom_t *parent,
            int birthdate,
            double step_size,
            void (*mutation_function) (real_chrom_t *, double),
            int (*is_valid) (real_chrom_t *))
{
  real_chrom_t *offspring;
  offspring = copy_real_chrom (parent, birthdate);
  (*mutation_function) (offspring, step_size);
  if (!(*is_valid) (offspring))
    {
      del_real_chrom (offspring);
      offspring = copy_real_chrom (parent, birthdate);
    }
  return offspring;
}

void
real_ev (real_pop_t *population,
         long generations,
         double step_size,
         void (*mutation_function) (real_chrom_t *, double),
         double (*fitness_function) (real_chrom_t *),
         int (*is_valid) (real_chrom_t *))
{
  stats_t *stats = init_stats ();
  apply_real_pop_fitness (population, fitness_function);
  reset_real_pop_stats (population, stats);
  do
    {
      int random_individual;
      real_chrom_t *parent, *offspring, *victim;
      long pop_size = population->size;

      random_individual = gsl_rng_uniform_int (rng, pop_size);
      parent = population->individuals[random_individual];
      population->birthcounter++;
      offspring = real_breed (parent, population->birthcounter, step_size, mutation_function, is_valid);
      offspring->fitness = (*fitness_function) (offspring);
      random_individual = gsl_rng_uniform_int (rng, pop_size);
      victim = population->individuals[random_individual];
      if (offspring->fitness > victim->fitness)
        {
          population->individuals[random_individual] = offspring;
          del_real_chrom (victim);
        }
      else
        del_real_chrom (offspring);
      population->generation++;
      reset_real_pop_stats (population, stats);
    }
  while (population->generation < generations);
  print_real_pop (population, stats);
  del_stats (stats);
}

#endif

