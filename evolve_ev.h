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

// EV: according to DE JONG's implementation

#ifndef __evolve_ev_h__
#define __evolve_ev_h__

#include "evolve.h"
#include "evolve_repr_real.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef evolve_ev_t
#define evolve_ev_t evolve_object_t
#endif
#ifndef evolve_del_ev
#define evolve_del_ev evolve_del_object
#endif

evolve_ev_t *
evolve_ev_init ();

evolve_real_chrom_t *
evolve_ev_breed (const evolve_object_t *);

void
evolve_ev_gaussian_mutation (const evolve_object_t *);

void
evolve_ev_delta_mutation (const evolve_object_t *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ev_h__
