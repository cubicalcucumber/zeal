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
  compiler->current_register = 0;
  compiler->error = false;
  parser_reset_input(compiler->parser, input);
}

static void generate_expression(Compiler* compiler, Fragment* fragment)
{
  parse(compiler->parser, fragment);
  fragment_add_code(fragment, ZEAL_OP_PRINT);
  fragment_add_code(fragment, ZEAL_OP_HALT);
}

/* Convert the previous token to an integer value. */
static Value create_integer(Compiler* compiler)
{
  int64_t as_int;
  if (!int64_from_token(compiler->parser->previous_token, &as_int))
    error_from_previous_token(compiler->parser,
                              "Compiler error: Integer out of range.\n");
  return value_from_integer(as_int);
}

void compiler_init(Compiler* compiler, Parser* parser)
{
  compiler->current_register = 0;
  compiler->parser = parser;
  compiler->parser->compiler = compiler;
}

void parse_and_compile(Compiler* compiler, const char* input,
                       Fragment* fragment)
{
  compiler_reset(compiler, input);
  generate_expression(compiler, fragment);
}

static Instruction binary_op_instruction(Compiler* compiler)
{
  return (compiler->current_register << 24) |
         ((compiler->current_register + 1) << 16) |
         (compiler->current_register << 8);
}

void generate_binary_op(Compiler* compiler, Token op_token, Fragment* fragment)
{
  compiler->current_register -= 2;
  if (op_token.type == ZEAL_PLUS_TOKEN)
    fragment_add_code(fragment, binary_op_instruction(compiler) | ZEAL_OP_ADD);
  else if (op_token.type == ZEAL_STAR_TOKEN)
    fragment_add_code(fragment, binary_op_instruction(compiler) | ZEAL_OP_MUL);
  compiler->current_register += 1;
}

void generate_integer(Compiler* compiler, Fragment* fragment)
{
  /* Put the integer into the constant pool. */
  size_t slot_index = fragment_add_data(fragment, create_integer(compiler));

  /* Emit code which loads the integer at its index into stack slot 0. */
  Instruction load = (((uint32_t)slot_index) << 24) |
                     (compiler->current_register << 8) | ZEAL_OP_LOAD;
  compiler->current_register += 1;
  fragment_add_code(fragment, load);
}
