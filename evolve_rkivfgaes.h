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

// Random-key Genetic Algorithm/Evolution Strategy with IVF (In Vitro Fertilization)

#ifndef __evolve_rk_ivf_ga_es_h__
#define __evolve_rk_ivf_ga_es_h__

#include "evolve.h"
#include "evolve_repr_real.h"
#include "evolve_rkgaes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef evolve_rkivfgaes_t
#define evolve_rkivfgaes_t evolve_object_t
#endif
#ifndef evolve_del_rkivfgaes
#define evolve_del_rkivfgaes evolve_del_object
#endif

evolve_rkivfgaes_t *
evolve_rkivfgaes_init ();

evolve_real_chrom_t **
evolve_rkivfgaes_parents_selection (const evolve_object_t *);

void
evolve_rkivfgaes_manipulation (const evolve_object_t *);

void
evolve_rkivfgaes_transference (const evolve_object_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_rk_ivf_ga_es_h__
