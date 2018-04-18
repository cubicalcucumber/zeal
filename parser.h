#ifndef zeal_parser_h
#define zeal_parser_h

#include "lexer.h"
#include "token.h"

typedef struct
{
  const char* input;
  Lexer lexer;
  Token current_token;
} Parser;

/* Parse the given input string. For now, there exist only a single parse rule:
 * read a number and nothing more.*/
void parse(Parser* parser, const char* input);

#endif
