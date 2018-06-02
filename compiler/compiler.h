#ifndef zeal_compiler_h
#define zeal_compiler_h

#include <stdbool.h>

#include "parser.h"
#include "../vm/fragment.h"

typedef struct Compiler
{
  Parser* parser;
  bool error;
  size_t current_register;
  /* The fragment we emit code and data into. */
  Fragment* fragment;
} Compiler;

void compiler_init(Compiler* compiler, Parser* parser);

/* Generate code and data for the given input string. */
void compile(Compiler* compiler, const char* input, Fragment* fragment);

/* Generate code and data for an integer object represented by the current
 * token. */
void generate_integer(Compiler* compiler);

void generate_binary_op(Compiler* compiler, Token op_token);

#endif
