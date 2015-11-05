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

#ifndef __evolve_library_h__
#define __evolve_library_h__

#include <stddef.h>
#include <proto.h>

#include "evolve_common.h"
#include "evolve_rng.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef evolve_object_t
#define evolve_object_t proto_object_t
#endif
#ifndef evolve_init_object
#define evolve_init_object proto_init_object
#endif
#ifndef evolve_del_object
#define evolve_del_object proto_del_object
#endif
#ifndef evolve_array_t
#define evolve_array_t proto_array_t
#endif
#ifndef evolve_init_array
#define evolve_init_array proto_init_array
#endif
#ifndef evolve_del_array
#define evolve_del_array proto_del_array
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_library_h__
