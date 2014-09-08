#undef NDEBUG
#ifndef _spec_h
#define _spec_h

#include <stdio.h>
#include <stdlib.h>

#include "../../lib/error_handling.h"
#include "../../lib/helpers.h"

#define CONSOLE_RED      "\e[31m"
#define CONSOLE_GREEN    "\e[38;5;40m"
#define CONSOLE_ORANGE   "\e[40;38;5;214m"
#define CONSOLE_RESET    "\e[0m"


#define spec_describe(S)               (printf("\n%s %s %s\n", CONSOLE_ORANGE, S, CONSOLE_RESET))
#define setup(S)                  printf("\n%s Starting specs: %s %s\n", CONSOLE_ORANGE, S, CONSOLE_RESET)
#define teardown()                printf("\n")

#define print_status(S, COLOR)    (printf("  %s%s %s\n", COLOR, S, CONSOLE_RESET))
#define print_success(S)          print_status(S, CONSOLE_GREEN)
#define print_failure(S)          print_status(S, CONSOLE_RED)
#define print_location()          (printf("file: '%s' line '%d'\n", __FILE__, __LINE__))

#define print_int_expectation(A, B)     (printf("Expected %d to be %d\n", A, B))
#define print_float_expectation(A, B)   (printf("Expected %f to be %f\n", A, B))
#define print_string_expectation(A, B)  (printf("Expected %s to be %s\n", A, B))


#define spec_equal(A, B, S)       (                                                         \
                                    ((A) == (B)) ?                                          \
                                    print_success(S) :                                      \
                                    print_failure(S) && print_location()                    \
                                  )

#define spec_strings_equal(A, B, S) (                                                                         \
                                      (strcmp(A, B)) == 0 ?                                                   \
                                      print_success(S) :                                                      \
                                      print_failure(S) && print_location() && print_string_expectation(A, B)  \
                                    )


// char  *message = NULL;
char  *spec_description = NULL;
int   specs_passed = 0;
int   specs_failed = 0;

#endif
