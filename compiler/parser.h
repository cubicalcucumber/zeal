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
  Token current_token;
  Token previous_token;
  /* The input string being parsed. */
  const char* input;
} Parser;

/* Reset the given parser instance, i.e. reset the error state and input
 * string. */
void parser_reset_input(Parser* parser, const char* input);

/* Parse an expression and invoke code generation into the current fragment. */
void parse(Parser* parser);

/* Report an error and display the current input string. */
void error(Parser* parser, const char* fmt, ...);

void error_from_previous_token(Parser* parser, const char* fmt, ...);

#endif
