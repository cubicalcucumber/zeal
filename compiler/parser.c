#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "parser.h"

/* Set the current token of the parser. */
static void set_current_token(Parser* parser, TokenType type)
{
  parser->current_token =
      (Token){.type = type,
              .beginning = parser->lexer.lexeme_beginning,
              .length = current_lexeme_length(&parser->lexer)};
  parser->lexer.lexeme_beginning = parser->lexer.current_char;
}

/* Lex a token. Currently only integer tokens are supported. */
static void lex_token(Parser* parser)
{
  /* First, eat away the leading whitespace. Then, use the current character to
   * decide which type of token we have to lex. */
  eat_leading_whitespace(&parser->lexer);
  char current = peek_char(&parser->lexer);

  if (isdigit(current))
  {
    read_digits(&parser->lexer);
    set_current_token(parser, ZEAL_INTEGER_TOKEN);
    return;
  }
  else if (current == '\0')
  {
    set_current_token(parser, ZEAL_EOF_TOKEN);
    return;
  }

  printf("Lexer error: unexpected character '%c'.\n", current);
  parser->error = true;
  set_current_token(parser, ZEAL_ERROR_TOKEN);
}

void expect(Parser* parser, TokenType type, const char* error)
{
  lex_token(parser);

  /* If a lexer error has been encountered, the current token is the error
   * token. Thus, the parser error is reported as well. */
  if (parser->current_token.type == type)
    return;

  printf("Parse error: %s.\n", error);
  parser->error = true;
}

void parser_set_input(Parser* parser, const char* input)
{
  parser->error = false;
  parser->input = input;
  parser->lexer.current_char = input;
  parser->lexer.lexeme_beginning = input;
}
