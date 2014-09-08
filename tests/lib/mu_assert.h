/*
  Taken from Zed Shaw's awesome 'Learn to Code C the Hard Way': http://c.learncodethehardway.org/book
  and modified a smidge
*/
#undef NDEBUG
#ifndef _muassert_h
#define _muassert_h

#include <stdio.h>
#include <stdlib.h>

#include "../../lib/error_handling.h"

#define mu_suite_start() char *message = NULL

#define mu_assert(test, message) if (!(test)) {               \
  log_err(message);                                           \
  return message;                                             \
}

#define mu_run_test(test) debug("\n-----%s", " " #test);      \
    message = test(); tests_run++; if (message) return message;

#define RUN_TESTS(name) int main(int argc, char *argv[]) {    \
    argc = 1;                                                 \
    debug("----- RUNNING: %s", argv[0]);                      \
    printf("----\nRUNNING: %s\n", argv[0]);                   \
    char *result = name();                                    \
    if (result != 0) {                                        \
        printf("\e[31mFAILED: %s\e[0m\n", result);            \
    }                                                         \
    else {                                                    \
        printf("\e[38;5;40mALL TESTS PASSED\e[0m\n");         \
    }                                                         \
    printf("Tests run: %d\n", tests_run);                     \
        exit(result != 0);                                    \
}


int tests_run;

#endif
