#ifndef zeal_zeal_h
#define zeal_zeal_h

#include "compiler/compiler.h"
#include "compiler/parser.h"
#include "vm/vm.h"

typedef struct
{
  Parser parser;
  Compiler compiler;
  VirtualMachine vm;
} Interpreter;

/* Initialize the interpreter. */
void interpreter_init(Interpreter* interpreter);

/* Parse, compile and run the given input string. */
void evaluate(Interpreter* interpreter, const char* input);

#endif
