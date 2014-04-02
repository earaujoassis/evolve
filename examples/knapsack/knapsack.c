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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/settings.h"

#include <evolve/evolve_ivfga.h>
#include <evolve/evolve_info.h>

#include "utils/fitness.c"
#include "utils/parser.c"

#define USAGE "Usage: knapsack <option> [instance-file-name]\n\
Options:\n\
-h\t\t\t\tPrint this help message and exit\n\
[instance-file-name]\t\tInstance files to be processed, found in P.C.Chu and\n\
\t\t\t\tJ.E.Beasley \"A genetic algorithm for the multidimensional\n\
\t\t\t\tknapsack problem\" (1997)\n\
\n"

int
main (int argc, char **argv)
{
  char *filename;
  int opt, instances, counter;
  FILE *instance_file;
  knapsack_t *problem;
  evolve_int_pop_t *population;
  evolve_stats_t *stats;

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
  instance_file = fopen (filename,"r");
  fscanf (instance_file, "%d\n", &instances);

  for(counter = 0; counter < instances; counter++)
    {
      problem = init_instance (instance_file);
      if (problem == NULL)
        return 0;

      set_problem (problem);
      population = evolve_init_int_pop (POP_SIZE);
      evolve_set_rng (SEED);
      evolve_random_int_pop (population, problem->items, &check);
      stats = evolve_int_ivfga (population, MAX_GEN, &fitness, &check);
      evolve_tear_rng ();
      printf ("\n");
      evolve_print_algo_info ("GA:", "Multidimensional 0-1 Knapsack Problem", MAX_GEN, POP_SIZE, SEED);
      evolve_print_int_pop (population, stats);
      printf ("\n");
      evolve_del_stats (stats);
      evolve_del_int_pop_indiv (population);
      evolve_del_int_pop (population);

      del_knapsack (problem);
    }
}
