// Copyright 2013-2015 Ewerton Assis
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

#ifndef __evolve_ioc_settings_h__
#define __evolve_ioc_settings_h__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
  size_t seed;
  size_t pop_size;
  size_t max_gen;
  void (*selection_function)(int,...);
  void (*breeding_function)(int,...);
  void (*solution_function)(int,...);
} evolve_ioc_settings_t;

evolve_ioc_settings_t **
evolve_ioc_init (size_t seed,
                 size_t pop_size,
                 size_t max_gen,
                 void (*selection_function)(int,...),
                 void (*breeding_function)(int,...),
                 void (*solution_function)(int,...));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_ioc_settings_h__
