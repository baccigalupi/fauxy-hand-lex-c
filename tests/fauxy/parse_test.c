#include "../lib/spec.h"

#include "../lib/parse.h"



void specs() {
  spec_setup("Parsing");

  // test single item statements
  // * number
  // * string
  // * regex
  // * atom
  // * id
  // * class name

  // * block

  spec_teardown();
}

run_all_specs(specs);
