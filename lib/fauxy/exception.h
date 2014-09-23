#ifndef __fauxy_exception
#define __fauxy_exception

typedef enum {
  FX_EX_MEMORY_ERROR,
  FX_EX_UNEXPECTED_TOKEN
} ExceptionCode;

typedef struct Exception {
  ExceptionCode code;
} Exception;

#define exception_code(E)       ((E)->code)
#define exception_free(E)       (pfree(E))

Exception *Exception_create(ExceptionCode code);

#endif
