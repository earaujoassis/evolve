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

#ifndef __evolve_defaults_h__
#define __evolve_defaults_h__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef XOVER_RATE
#define XOVER_RATE         70.0
#endif
#ifndef MUTATION_RATE
#define MUTATION_RATE      0.15
#endif
#ifndef NPOINTS_XOVER
#define NPOINTS_XOVER           4
#endif
#ifndef HEAD_TOSSING
#define HEAD_TOSSING            70.0
#endif
#ifndef TOURNAMENT_SIZE
#define TOURNAMENT_SIZE         5
#endif
#ifndef REGENERATED_PERCENTAGE
#define REGENERATED_PERCENTAGE  0.20
#endif

#ifndef GENES_OUTPUT_LIMIT
#define GENES_OUTPUT_LIMIT      10
#endif

#ifndef IVFGA_DEFAULT_XOVER_POINTS
#define IVFGA_DEFAULT_XOVER_POINTS      10
#endif
#ifndef IVFGA_SUPER_PARENTS
#define IVFGA_SUPER_PARENTS             population->size * 0.5
#endif

#ifndef IVFGA_SUPER_PARENTS
#define IVFGA_SUPER_PARENTS             population->size * 0.25
#endif
#ifndef IVFGA_CUTOFFS_UP_BOUND
#define IVFGA_CUTOFFS_UP_BOUND          0.3
#endif
#ifndef IVFGA_CUTOFFS_BOTTOM_BOUND
#define IVFGA_CUTOFFS_BOTTOM_BOUND      0.2
#endif

#ifndef PROB_EXCHANGE
#define PROB_EXCHANGE           35.0 // %
#endif
#ifndef PROB_INSERTION
#define PROB_INSERTION          35.0 // %
#endif
#ifndef PROB_INVERSION
#define PROB_INVERSION          10.0 // %
#endif
#ifndef PROB_SHIFTING
#define PROB_SHIFTING           20.0 // %
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_defaults_h__
