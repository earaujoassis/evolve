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

// Random-key Genetic Algorithm/Evolution Strategy

#ifndef __evolve_hybrid_real_rk_ga_es_h__
#define __evolve_hybrid_real_rk_ga_es_h__

#include "evolve.h"
#include "evolve_repr_real.h"
#include "evolve_gaes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef evolve_rkgaes_t
#define evolve_rkgaes_t evolve_object_t
#endif
#ifndef evolve_del_rkgaes
#define evolve_del_rkgaes evolve_del_object
#endif

evolve_rkgaes_t *
evolve_rkgaes_init ();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_hybrid_real_rk_ga_es_h__
