#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "token.h"
#include "../utils.h"
#include "../vm/value.h"

/* Convert the given token to a 64-bit integer. Return true on success. */
static bool int64_from_token(Token token, int64_t* result)
{
  char* end = ((char*)token.beginning) + token.length;
  *result = strtol(token.beginning, &end, 10);
  return errno != ERANGE;
}

/* Reset the compilers error state and parser. */
static void compiler_reset(Compiler* compiler, const char* input)
{
  compiler->error = false;
  parser_reset(compiler->parser, input);
}

static void generate_expression(Compiler* compiler, Fragment* fragment)
{
  parse_expression(compiler->parser, fragment);
  /* Print the value in register 0 and halt the execution. */
  fragment->code.instructions[1] = 1;
  fragment->code.instructions[2] = 2;
}

/* Convert the current token to an integer value. */
static Value create_integer(Compiler* compiler)
{
  int64_t as_int;
  if (!int64_from_token(compiler->parser->current_token, &as_int))
  {
    printf("Compiler error: Integer out of range.\n");
    compiler->error = true;
  }
  return value_from_integer(as_int);
}

void connect_compiler_and_parser(Compiler* compiler, Parser* parser)
{
  compiler->parser = parser;
  compiler->parser->compiler = compiler;
}

void parse_and_compile(Compiler* compiler, const char* input,
                       Fragment* fragment)
{
  compiler_reset(compiler, input);
  generate_expression(compiler, fragment);
}

void generate_integer(Compiler* compiler, Fragment* fragment)
{
  /* For now, put the number value into slot 0 of the constant pool and emit
   * code to load the constant into register 0. */
  fragment->data.values[0] = create_integer(compiler);
  fragment->code.instructions[0] = 0;
}
