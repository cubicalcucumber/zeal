#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "compiler/compiler.h"
#include "repl.h"
#include "vm/fragment.h"
#include "vm/vm.h"

/* Parse, compile and run the given input string. */
static void evaluate(Parser* parser, Compiler* compiler, VirtualMachine* vm,
                     const char* input)
{
  Fragment fragment;
  generate(compiler, input, &fragment);

  if (compiler->error)
    printf("Errors occurred while compiling.\n");
  else
    run(vm, &fragment);
}

int32_t main()
{
  char* line = NULL;
  bool is_running = true;

  Parser parser;
  Compiler compiler;
  VirtualMachine vm;

  compiler_init(&compiler, &parser);

  /* The REPL loop. */
  do
  {
    display_prompt();
    line = read_line(stdin);

    if (strcmp(line, ":q") == 0)
      is_running = false;
    else
      evaluate(&parser, &compiler, &vm, line);

    free(line);
  } while (is_running);

  return 0;
}
