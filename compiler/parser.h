#ifndef zeal_parser_h
#define zeal_parser_h

#include <stdbool.h>

#include "lexer.h"
#include "token.h"

typedef struct
{
  const char* input;
  Lexer lexer;
  Token current_token;
} Parser;

/* Setup the pointers into the input string. */
void parser_set_input(Parser* parser, const char* input);

/* Expect the next token to be of a given type. If the types mismatch return
 * the given error message. */
bool expect(Parser* parser, TokenType type, const char* error);

#endif
