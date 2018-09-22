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
  vm_init(&interpreter->vm);
}

Value interpreter_get_result(Interpreter* interpreter)
{
  return interpreter->vm.stack[ZEAL_STACK_SIZE - 1];
}

EvaluationResult evaluate(Interpreter* interpreter, const char* input)
{
  Fragment fragment;
  fragment_init(&fragment);
  compile(&interpreter->compiler, input, &fragment);
  if (interpreter->compiler.error)
    return ZEAL_EVALUATION_ERROR;
  run(&interpreter->vm, &fragment);
  return ZEAL_EVALUATION_SUCCESS;
}
