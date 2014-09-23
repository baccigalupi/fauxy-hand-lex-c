#include "exception.h"

#include "../bricks/error_handling.h"

#include <stdlib.h>

Exception *Exception_create(ExceptionCode code) {
  Exception *e = calloc(1, sizeof(Exception));
  check_mem(e);
  exception_code(e) = code;

  return e;
error:
  return NULL;
}
