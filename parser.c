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

/* Expect the next token to be of a given type. If the types mismatch return
 * the given error message. */
static bool expect(Parser* parser, TokenType type, const char* error)
{
  lex_next_token(parser);

  if (parser->current_token.type == type)
    return true;

  printf("Parse error: %s.\n", error);
  return false;
}

void parse(Parser* parser, const char* input)
{
  /* Setup the pointer into the input string. */
  parser->input = input;
  parser->lexer.current_char = input;
  parser->lexer.lexeme_beginning = input;

  if (!expect(parser, ZEAL_INTEGER_TOKEN, "expected integer"))
    return;
  expect(parser, ZEAL_EOF_TOKEN, "expected eof after integer");
}
