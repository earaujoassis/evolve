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

// Genetic Algorithm with IVF (In Vitro Fertilization)

#ifndef __evolve_ga_ivf_h__
#define __evolve_ga_ivf_h__

#include "evolve.h"
#include "evolve_repr_integer.h"
#include "evolve_ga.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef evolve_ivfga_t
#define evolve_ivfga_t evolve_object_t
#endif
#ifndef evolve_del_ivfga
#define evolve_del_ivfga evolve_del_object
#endif

evolve_ivfga_t *
evolve_ivfga_init ();

evolve_int_chrom_t **
evolve_ivfga_parents_selection (const evolve_object_t *);

void
evolve_ivfga_manipulation (const evolve_object_t *);

void
evolve_ivfga_transference (const evolve_object_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ga_ivf_h__
