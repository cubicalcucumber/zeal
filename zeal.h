#ifndef zeal_zeal_h
#define zeal_zeal_h

#include "compiler/compiler.h"
#include "compiler/parser.h"
#include "vm/value.h"
#include "vm/vm.h"

typedef struct
{
  Parser parser;
  Compiler compiler;
  VirtualMachine vm;
} Interpreter;

typedef enum
{
  ZEAL_EVALUATION_SUCCESS,
  ZEAL_EVALUATION_ERROR
} EvaluationResult;

/* Initialize the interpreter. */
void interpreter_init(Interpreter* interpreter);

/* Get the result of the expression which has been evaluated last. */
Value interpreter_get_result(Interpreter* interpreter);

/* Parse, compile and run the given input string. */
EvaluationResult evaluate(Interpreter* interpreter, const char* input);

#endif
