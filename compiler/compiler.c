#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "../utils.h"
#include "../vm/value.h"

/* Convert the given token to a 64-bit integer. Return true on success. */
static bool int64_from_token(Token token, int64_t* result)
{
  char* end = ((char*)token.beginning) + token.length;
  *result = strtol(token.beginning, &end, 10);
  return errno != ERANGE;
}

/* Convert the current token to a number value. */
static Value create_integer(Compiler* compiler)
{
  int64_t as_int;
  if (!int64_from_token(compiler->parser.current_token, &as_int))
  {
    printf("Compiler error: Integer out of range.\n");
    compiler->error = true;
  }
  return value_from_integer(as_int);
}

/* Generate code and data for a given integer value. */
static void generate_integer(Compiler* compiler, Value integer,
                             Fragment* fragment)
{
  /* For now, put the number value into slot 0 of the constant pool and emit
   * code to load the constant into register 0. */
  fragment->data.values[0] = integer;
  fragment->code.instructions[0] = 0;
}

void compiler_reset(Compiler* compiler) { compiler->error = false; }

void compile_expression(Compiler* compiler, const char* input,
                        Fragment* fragment)
{
  parser_set_input(&compiler->parser, input);

  expect(&compiler->parser, ZEAL_INTEGER_TOKEN, "expected integer");
  if (compiler->parser.error)
  {
    compiler->error = true;
    return;
  }

  Value integer = create_integer(compiler);
  generate_integer(compiler, integer, fragment);

  expect(&compiler->parser, ZEAL_EOF_TOKEN, "expected eof after integer");
  if (compiler->parser.error)
  {
    compiler->error = true;
    return;
  }

  /* Print the value in register 0 and halt the execution. */
  fragment->code.instructions[1] = 1;
  fragment->code.instructions[2] = 2;

  return;
}
