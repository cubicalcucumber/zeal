#ifndef zeal_parser_h
#define zeal_parser_h

#include <stdbool.h>

#include "lexer.h"
#include "token.h"
#include "../vm/fragment.h"

typedef struct
{
  /* The compiler which is called by the parser for code generation. */
  struct Compiler* compiler;
  Lexer lexer;
  /* The token encountered last. */
  Token current_token;
  /* The input string being parsed. */
  const char* input;
} Parser;

/* Reset the given parser instance, i.e. reset the error state and input
 * string. */
void parser_reset_input(Parser* parser, const char* input);

/* Parse an expression and invoke code generation into the given fragment. */
void parse_expression(Parser* parser, Fragment* fragment);

/* Report an error and display the current input string. */
void error(Parser* parser, const char* fmt, ...);

#endif
