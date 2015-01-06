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

#ifndef __evolve_intstring_h__
#define __evolve_intstring_h__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void
evolve_intstring_reverse (unsigned short int *string,
                          size_t size);

unsigned short int *
evolve_intstring_slice (const unsigned short int *string,
                        size_t begin,
                        size_t end);

long int
evolve_intstring_decode (const unsigned short int *string,
                         size_t size);

double
evolve_intstring_normalize (long int value,
                            double min,
                            double max,
                            unsigned int k);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __evolve_intstring_h__

