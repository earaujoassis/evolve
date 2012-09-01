// Copyright 2011 Éwerton Assis
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

#include <math.h>
#include <stdlib.h>

void
intstring_reverse (unsigned short int *string,
                   size_t size)
{
  size_t i, j;
  unsigned short int aux;
  for (i = 0, j = size - 1; i < j; i++, j--)
    {
      aux = string[i];
      string[i] = string[j];
      string[j] = aux;
    }
}

unsigned short int *
intstring_slice (const unsigned short int *string,
                 size_t begin,
                 size_t end)
{
  size_t i;
  unsigned short int *slice = (unsigned short int *) calloc (end - begin + 1, \
    sizeof (unsigned short int));
  for (i = 0; i < (end - begin + 1); i++)
    slice[i] = string[begin + i];
  return slice;
}

long int
intstring_decode (const unsigned short int *string,
                  size_t size)
{
  size_t i, j;
  long int value = 0;
  for (i = 0, j = size - 1; i < size; i++, j--)
    if (string[j])
      value += (long int) pow (2, i);
  return value;
}

double
intstring_normalize (long int value,
                     double min,
                     double max,
                     unsigned int k)
{
  return (((double) value * (max - min)) / (pow (2, k) - 1)) + min;
}

#endif

