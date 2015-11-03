// Copyright 2015 Ewerton Assis
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

#ifndef __test_utils_h__
#define __test_utils_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

static size_t success_counter = 0;
static size_t errors_counter = 0;
static size_t skipped_counter = 0;
static char *description = "";

#define fail_place fprintf (stderr, "%s:%d: ", __FILE__, __LINE__)

#define fail(msg)                       \
    do                                  \
      {                                 \
        fail_place;                     \
        fprintf (stderr, "%s\n", msg);  \
        exit (1);                       \
      }                                 \
    while (0)

#define should_be_true(x)                          \
    do                                             \
      {                                            \
        spec_eval (__FILE__, __LINE__, (x));  \
      }                                            \
    while (0)

#define should_be_false(x) should_be_true (!x)
#define should_equal(x, y) should_be_true (x == y)

void
run_tests ();

void
describe (char *message)
{
  description = message;
}

void
skip (char *message)
{
  skipped_counter++;
  printf ("\033[93m·\033[0m");
}

static void
spec_eval (char *filename,
           int line_number,
           int is_success)
{
  if (is_success)
    {
      success_counter++;
      printf ("\033[1m\033[92m·\033[0m");
    }
  else
    {
      errors_counter++;
      fprintf (stderr, "\n   \033[1m%s\033[0m\n", description);
      fprintf (stderr, "     \033[91mFailed at %s:%d\033[0m\n", filename, line_number);
    }
}

int
main ()
{
  run_tests ();

  printf ("\n\n\033[93mSkipped: %zu.\033[0m \033[92mSuccess: %zu.\033[0m \033[91mErrors: %zu.\033[0m\n",
    skipped_counter, success_counter, errors_counter);

  return errors_counter;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __test_utils_h__
