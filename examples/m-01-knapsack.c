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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <evolve/evolve_settings.h>
#include <evolve/evolve_ivfga.h>
#include <evolve/evolve_info.h>

#define SEED               987654
#define MAX_GEN            20
#define POP_SIZE           50
#define USAGE "Usage: knapsack <option> [instance-file-name]\n\
Options:\n\
-h\t\t\t\tPrint this help message and exit\n\
[instance-file-name]\t\tInstance files to be processed, found in P.C.Chu and\n\
\t\t\t\tJ.E.Beasley \"A genetic algorithm for the multidimensional\n\
\t\t\t\tknapsack problem\" (1997)\n\
\n"

typedef struct {
  size_t items;
  size_t compartments;
  unsigned long int *values;
  unsigned long int *capacity;
  unsigned long int **weights;
} knapsack_t;

knapsack_t *
init_knapsack (size_t items,
               size_t compartments,
               unsigned long int *values,
               unsigned long int **weights,
               unsigned long int *capacity)
{
  knapsack_t *problem;
  problem = (knapsack_t *) malloc (sizeof (knapsack_t));
  problem->items = items;
  problem->values = values;
  problem->compartments = compartments;
  problem->capacity = capacity;
  problem->weights = weights;
  return problem;
}

void
del_knapsack (knapsack_t *problem)
{
  if (problem == NULL)
    return;
  size_t i;
  for (i = 0; problem->weights[i] != NULL; i++)
    free (problem->weights[i]);
  free (problem->weights);
  free (problem->values);
  free (problem->capacity);
  free (problem);
}

knapsack_t *
init_instance (FILE *stream)
{
  size_t i, j, k;
  size_t items, compartments;
  unsigned long int *values, **weights, *capacity;
  knapsack_t *problem;

  items = 0;
  compartments = 0;
  fscanf (stream, "%zd %zd %*d", &items, &compartments);
  if (!items || !compartments)
    return NULL;
  values = (unsigned long int *) calloc (items, sizeof (unsigned long int));
  for (i = 0; i < items; i++)
    fscanf(stream, " %ld ", &values[i]);
  weights = (unsigned long int **) calloc (compartments + 1, sizeof (unsigned long int *));
  weights[compartments] = NULL;
  for (i = 0; i < compartments; i++)
    {
      weights[i] = (unsigned long int *) calloc (items, sizeof (unsigned long int));
      for (j = 0; j < items; j++)
        fscanf (stream, " %ld ", &weights[i][j]);
    }
  capacity = (unsigned long int *) calloc (compartments, sizeof (unsigned long int));
  for (i = 0; i < compartments; i++)
    fscanf (stream, " %ld ", &capacity[i]);
  problem = init_knapsack (items, compartments, values, weights, capacity);
  return problem;
}

static knapsack_t *local_problem;

void
set_problem (knapsack_t *problem)
{
  local_problem = problem;
}

int
is_subjected_original (evolve_int_chrom_t *chrom)
{
  size_t compartment, item;
  unsigned long int total_weight;
  for (compartment = 0; compartment < local_problem->compartments; compartment++)
    {
      total_weight = 0;
      for (item = 0; item < local_problem->items; item++)
        if (chrom->vector[item])
          total_weight += local_problem->weights[compartment][item];
      if (total_weight > local_problem->capacity[compartment])
        return 0;
    }
  return 1;
}

int
is_subjected (evolve_int_chrom_t *chrom)
{
  size_t item, compartment;
  size_t compartments = local_problem->compartments;
  size_t items = local_problem->items;
  unsigned long int *weight_per_compartment;
  weight_per_compartment = (unsigned long int *) calloc (compartments, sizeof (unsigned long int));
  for (compartment = 0; compartment < compartments; compartment++)
    {
      weight_per_compartment[compartment] = 0;
      for (item = 0; item < items; item++)
        if (chrom->vector[item])
          weight_per_compartment[compartment] += local_problem->weights[compartment][item];
    }
  for (compartment = 0; compartment < compartments; compartment++)
    if (weight_per_compartment[compartment] > local_problem->capacity[compartment])
      {
        free (weight_per_compartment);
        return 0;
      }
  free (weight_per_compartment);
  return 1;
}

void
mutate (evolve_int_chrom_t *chrom)
{
  size_t point = gsl_rng_uniform_int (rng, chrom->size - 1);
  while (!chrom->vector[point]) /* chrom->vector[point] != 1 */
    point = gsl_rng_uniform_int (rng, chrom->size - 1);
  chrom->vector[point] = 0;
}

double
fitness (evolve_int_chrom_t *chrom)
{
  size_t i;
  unsigned long int fitness = 0;
  for (i = 0; i < local_problem->items; i++)
    if (chrom->vector[i])
      fitness += local_problem->values[i];
  return (double) fitness;
}

int
check (evolve_int_chrom_t *chrom)
{
  while (!is_subjected_original (chrom))
    mutate (chrom);
  return 1;
}

int
main (int argc, char **argv)
{
  char *filename;
  int opt, instances, counter;
  FILE *instance_file;
  knapsack_t *problem;
  evolve_int_pop_t *population;
  evolve_stats_t *stats;
  evolve_settings_t *settings;
  evolve_object_t *solution;

  size_t generations = MAX_GEN;
  size_t xover_points = 5;
  size_t tournament_size = (size_t) POP_SIZE / 2;
  size_t selection_size = POP_SIZE;
  size_t ivf_num_parents = (size_t) POP_SIZE / 2;
  double xover_rate = 0.7;
  double recombination_parameterized = 0.8;
  double mutation_rate = 0.8;

  if (argc < 2)
    {
      fprintf (stderr, USAGE);
      return 0;
    }
  while ((opt = getopt (argc, argv, "h")) != -1)
    {
      switch (opt)
        {
          case 'h':
            fprintf (stdout, USAGE);
            return 0;
          case '?':
            if (isprint (optopt))
              fprintf (stderr, "Unknown option '-%c'.\n", optopt);
            else
              fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
            fprintf (stderr, USAGE);
            return 1;
          default:
            abort ();
        }
    }
  if (optind == argc)
    {
      fprintf (stderr, "Error: missing [file-name] element\n");
      fprintf (stderr, USAGE);
      return 1;
    }
  filename = argv[optind];
  instance_file = fopen (filename, "r");
  fscanf (instance_file, "%d\n", &instances);

  for(counter = 0; counter < instances; counter++)
    {
      problem = init_instance (instance_file);
      if (problem == NULL)
        return 0;

      set_problem (problem);
      evolve_set_rng (SEED);
      population = evolve_init_int_pop (POP_SIZE);
      evolve_random_int_pop (population, problem->items, &check);

      settings = evolve_settings_init ();
      settings->set_own_property (settings, "population", population);
      settings->set_own_property (settings, "generations", &generations);
      settings->set_own_property (settings, "fitness", &fitness);
      settings->set_own_property (settings, "breed", &evolve_ga_breed);
      settings->set_chain (settings, "strategies.check_chrom", &check);
      settings->set_chain (settings, "strategies.recombination_strategy", &evolve_ga_npoint_xover);
      settings->set_chain (settings, "strategies.recombination.num_points", &xover_points);
      settings->set_chain (settings, "strategies.recombination.xover_rate", &xover_rate);
      settings->set_chain (settings, "strategies.recombination.parameterized", &recombination_parameterized);
      settings->set_chain (settings, "strategies.mutation_strategy", &evolve_ga_per_int_mutation);
      settings->set_chain (settings, "strategies.mutation.chance", &mutation_rate);
      settings->set_chain (settings, "strategies.selection_strategy", &evolve_ga_roulette_wheel_selection);
      settings->set_chain (settings, "strategies.selection.tournament_size", &tournament_size);
      settings->set_chain (settings, "strategies.selection.total_size", &selection_size);
      settings->set_chain (settings, "strategies.replacement_policy", &evolve_ga_elitist_policy_replacement);
      settings->set_chain (settings, "strategies.ivf.recombination_strategy", &evolve_ga_npoint_xover);
      settings->set_chain (settings, "strategies.ivf.recombination.num_points", &xover_points);
      settings->set_chain (settings, "strategies.ivf.num_parents", &ivf_num_parents);

      solution = evolve_ivfga_init ();
      stats = (evolve_stats_t *) solution->execute_property (solution, "solution", settings);

      printf ("\n");
      evolve_print_algo_info ("GA:", "Multidimensional 0-1 Knapsack Problem", MAX_GEN, POP_SIZE, SEED);
      evolve_print_int_pop (population, stats);
      printf ("\n");

      evolve_tear_rng ();
      evolve_del_settings (settings);
      evolve_del_object (solution);
      evolve_del_stats (stats);
      evolve_del_int_pop_indiv (population);
      evolve_del_int_pop (population);

      del_knapsack (problem);
    }

  fclose (instance_file);

  return 0;
}
