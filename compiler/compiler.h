#ifndef zeal_compiler_h
#define zeal_compiler_h

#include <stdbool.h>
#include <stdint.h>

#include "parser.h"
#include "../vm/fragment.h"

typedef struct Compiler
{
  Parser* parser;
  /* Is set to true whenever a lexer, parser or compiler error has been
   * detected. */
  bool error;
  uint8_t current_register;
  /* The fragment we emit code and data into. */
  Fragment* fragment;
} Compiler;

void compiler_init(Compiler* compiler, Parser* parser);

/* Generate code and data for the given input string. */
void compile(Compiler* compiler, const char* input, Fragment* fragment);

/* Generate code and data for an integer object represented by the current
 * token. */
void compile_integer(Compiler* compiler);

/* Generate code for binary operators like + and -. */
void compile_binary_operator(Compiler* compiler, Token op_token);

#endif
