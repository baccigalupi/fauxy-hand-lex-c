#ifndef __fauxy_parse
#define __fauxy_parse

#include "stack.h"

#define stack_add_statement(SK, SM)   SM = Statement_create();                 \
                                      stack_push(SK, SM);

void parse_text(ParseState *state, Stack *stack);

#endif
