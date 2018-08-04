#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "compiler/compiler.h"
#include "vm/fragment.h"
#include "vm/vm.h"
#include "zeal.h"

void interpreter_init(Interpreter* interpreter)
{
  compiler_init(&interpreter->compiler, &interpreter->parser);
}

void evaluate(Interpreter* interpreter, const char* input)
{
  Fragment fragment;
  fragment_init(&fragment);
  compile(&interpreter->compiler, input, &fragment);
  if (!interpreter->compiler.error)
    run(&interpreter->vm, &fragment);
}
