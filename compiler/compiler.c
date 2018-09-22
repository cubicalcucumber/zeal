#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "token.h"
#include "../utils.h"
#include "../vm/value.h"

/* Convert the given token to a 64-bit integer. Return true on success. */
static void int64_from_token(Token token, int64_t* result)
{
  char* end = ((char*) token.beginning) + token.length;
  *result = strtoll(token.beginning, &end, 10);
}

/* Check for the ERANGE error and report compiler error if encountered. */
static void check_for_out_of_range_error(Compiler* compiler)
{
  if (errno == ERANGE)
  {
    errno = 0;
    error_from_previous_token(compiler->parser,
                              "Compiler error: integer out of range.\n");
  }
}

/* Convert the previous token to an integer value. */
static Value create_integer(Compiler* compiler)
{
  int64_t as_int;
  int64_from_token(compiler->parser->previous_token, &as_int);
  check_for_out_of_range_error(compiler);
  return value_from_integer(as_int);
}

/* Reset the compilers state and setup the current fragment. */
static void reset(Compiler* compiler, const char* input, Fragment* fragment)
{
  compiler->error = false;
  compiler->current_register = 0;
  compiler->fragment = fragment;
}

static void expression(Compiler* compiler)
{
  parse(compiler->parser);
  fragment_add_code(compiler->fragment, ZEAL_OP_HALT);
}

void compiler_init(Compiler* compiler, Parser* parser)
{
  compiler->current_register = 0;
  compiler->parser = parser;
  compiler->parser->compiler = compiler;
}

void compile(Compiler* compiler, const char* input, Fragment* fragment)
{
  reset(compiler, input, fragment);
  parser_reset_input(compiler->parser, input);
  expression(compiler);
}

static Instruction binary_op_instruction(Compiler* compiler)
{
  return (compiler->current_register << 24) |
         ((compiler->current_register + 1) << 16) |
         (compiler->current_register << 8);
}

void compile_binary_operator(Compiler* compiler, Token op_token)
{
  compiler->current_register -= 2;
  if (op_token.type == ZEAL_PLUS_TOKEN)
    fragment_add_code(compiler->fragment,
                      binary_op_instruction(compiler) | ZEAL_OP_ADD);
  else if (op_token.type == ZEAL_MINUS_TOKEN)
    fragment_add_code(compiler->fragment,
                      binary_op_instruction(compiler) | ZEAL_OP_SUB);
  else if (op_token.type == ZEAL_STAR_TOKEN)
    fragment_add_code(compiler->fragment,
                      binary_op_instruction(compiler) | ZEAL_OP_MUL);
  compiler->current_register += 1;
}

void compile_integer(Compiler* compiler)
{
  /* Put the integer into the constant pool. */
  size_t slot_index =
      fragment_add_data(compiler->fragment, create_integer(compiler));

  /* Emit code which loads the integer at its index into stack slot 0. */
  Instruction load = (((uint32_t) slot_index) << 24) |
                     (compiler->current_register << 8) | ZEAL_OP_LOAD;
  compiler->current_register += 1;
  fragment_add_code(compiler->fragment, load);
}
