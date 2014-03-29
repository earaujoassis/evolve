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

#ifndef __evolve_o_rkgaes_h__
#define __evolve_o_rkgaes_h__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum { proportional, tournament, ranking } selection_method_t;
typedef enum { onepoint, npoint, uniform } recombination_method_t;
typedef enum { regenerated, permutation, regenerated_permutation } mutation_method_t;

typedef struct {
  selection_method_t selection;
  recombination_method_t recombination;
  mutation_method_t mutation;
} rkgaes_options_t;

rkgaes_options_t *global_options;

void
init_options (selection_method_t selection,
              recombination_method_t recombination,
              mutation_method_t mutation);

void
destroy_options ();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_o_rkgaes_h__
