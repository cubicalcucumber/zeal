#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "compiler/compiler.h"
#include "repl.h"
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
  parse_and_compile(&interpreter->compiler, input, &fragment);
  if (!interpreter->compiler.error)
    run(&interpreter->vm, &fragment);
}

int32_t main()
{
  Repl repl;
  repl_init(&repl);
  repl_run(&repl);
  return 0;
}
