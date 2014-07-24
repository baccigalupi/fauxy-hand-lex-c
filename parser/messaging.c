#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "messaging.h"

void exception(char *message, char *thing) {
  int length = strlen(message) + strlen(thing) + 5;
  char full_message[length];

  strcat(full_message, message);
  strcat(full_message, ": ");
  strcat(full_message, thing);

  perror(full_message);
  exit(EXIT_FAILURE);
}

void debug_pointer(void *ptr, char *message) {
  printf("Pointer: %-35s %016" PRIxPTR "\n", message, (uintptr_t)(ptr));
}
