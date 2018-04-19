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

/* Lex the next token. Currently only integer tokens are supported. */
static bool lex_next_token(Parser* parser)
{
  /* First, eat away the leading whitespace. Then, use the current character to
   * decide which type of token we have to lex. */
  eat_leading_whitespace(&parser->lexer);
  char current = peek_char(&parser->lexer);

  if (isdigit(current))
  {
    read_digits(&parser->lexer);
    set_current_token(parser, ZEAL_INTEGER_TOKEN);
    return true;
  }
  else if (current == '\0')
  {
    set_current_token(parser, ZEAL_EOF_TOKEN);
    return true;
  }

  printf("Lexer error: unexpected character '%c'.\n", current);
  return false;
}

bool expect(Parser* parser, TokenType type, const char* error)
{
  /* If a lexer error has been encountered, the expected token could not be
   * lexed. Hence, the report the parse error as well. */
  if (!lex_next_token(parser))
  {
    printf("Parse error: %s.\n", error);
    return false;
  }

  if (parser->current_token.type == type)
    return true;

  printf("Parse error: %s.\n", error);
  return false;
}

void parser_set_input(Parser* parser, const char* input)
{
  parser->input = input;
  parser->lexer.current_char = input;
  parser->lexer.lexeme_beginning = input;
}
