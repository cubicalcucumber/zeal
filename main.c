#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "compiler/compiler.h"
#include "repl.h"
#include "vm/fragment.h"
#include "vm/vm.h"

/* Parse, compile and run the given input string. */
static void evaluate(Compiler* compiler, VirtualMachine* vm, Fragment* fragment,
                     const char* input)
{
  CompileResult result = compile_number(compiler, input, fragment);
  if (result == ZEAL_COMPILE_OK)
    run(vm, fragment);
}

int32_t main()
{
  char* line = NULL;
  bool is_running = true;

  Compiler compiler;
  VirtualMachine vm;
  Fragment fragment;

  /* The REPL loop. */
  do
  {
    display_prompt();
    line = read_line(stdin);

    if (strcmp(line, ":q") == 0)
      is_running = false;
    else
      evaluate(&compiler, &vm, &fragment, line);

    free(line);
  } while (is_running);

  return 0;
}
