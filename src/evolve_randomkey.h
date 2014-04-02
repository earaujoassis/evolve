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

#ifndef __evolve_randomkey_h__
#define __evolve_randomkey_h__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

unsigned int *
evolve_integer_series (size_t dimension,
                       const evolve_real_chrom_t *chrom);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_randomkey_h__
