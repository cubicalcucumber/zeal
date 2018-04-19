#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "utils.h"
#include "value.h"

/* Convert the current token to a number value. */
static Value value_from_integer_token(Parser* parser)
{
  Token number_token = parser->current_token;
  char* end = ((char*)number_token.beginning) + number_token.length;
  int64_t as_int = strtol(number_token.beginning, &end, 10);

  if (errno == ERANGE)
    printf("Lexer error: Integer is too large.\n");
  return value_integer(as_int);
}

CompileResult compile_number(Compiler* compiler, const char* input,
                              Fragment* fragment)
{
  parser_set_input(&compiler->parser, input);

  if (!expect(&compiler->parser, ZEAL_INTEGER_TOKEN, "expected integer"))
    return ZEAL_COMPILE_ERROR;

  Value number = value_from_integer_token(&compiler->parser);

  if (!expect(&compiler->parser, ZEAL_EOF_TOKEN, "expected eof after integer"))
    return ZEAL_COMPILE_ERROR;

  /* Put the number value into slot 0 of the constant pool. */
  fragment->data.values[0] = number;
  /* Load a program into the code section which loads the constant into
   * register 0, prints it and halts the execution:
   *   LOAD 0 -> 0
   *   PRINT 0
   *   HALT
   */
  fragment->code.instructions[0] = 0;
  fragment->code.instructions[1] = 1;
  fragment->code.instructions[2] = 2;

  return ZEAL_COMPILE_OK;
}
