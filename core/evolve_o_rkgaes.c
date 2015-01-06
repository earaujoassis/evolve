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

#include <stdlib.h>

#include "evolve_o_rkgaes.h"

void
evolve_init_rkgaes_options (evolve_selection_method_t selection,
                            evolve_recombination_method_t recombination,
                            evolve_mutation_method_t mutation)
{
  global_options = (evolve_rkgaes_options_t *) malloc (sizeof (evolve_rkgaes_options_t));
  global_options->selection = selection;
  global_options->recombination = recombination;
  global_options->mutation = mutation;
}

void
evolve_destroy_rkgaes_options ()
{
  free (global_options);
}
