#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "compiler.h"
#include "parser.h"

static const char* token_type_to_string(TokenType type)
{
  if (type == ZEAL_ERROR_TOKEN)
    return "lexer error";
  else if (type == ZEAL_INTEGER_TOKEN)
    return "integer";
  else
    return "end of input";
}

static void print_n_times(size_t n, char c)
{
  for (size_t i = 0; i < n; ++i)
    printf("%c", c);
}

static void println_n_times(size_t n, char c)
{
  print_n_times(n, c);
  puts("");
}

static void display_input_and_highlight_token(Parser* parser, Token token)
{
  printf(" | %s\n", parser->input);
  print_n_times(3 + token.beginning - parser->input, ' ');
  /* Underline current lexer position if lexeme is of length 0. This is useful
   * for error and eof tokens. */
  println_n_times(token.length == 0 ? 1 : token.length, '~');
}

static void mark_lexeme_beginning(Parser* parser)
{
  parser->lexer.lexeme_beginning = parser->lexer.current_char;
}

/* Set the current token of the parser. */
static void set_current_token(Parser* parser, TokenType type)
{
  parser->current_token =
      (Token){.type = type,
              .beginning = parser->lexer.lexeme_beginning,
              .length = current_lexeme_length(&parser->lexer)};
  mark_lexeme_beginning(parser);
}

void error(Parser* parser, const char* fmt, ...)
{
  if (parser->compiler->error)
    return;
  parser->compiler->error = true;
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  display_input_and_highlight_token(parser, parser->current_token);
}

static void lex_digit(Parser* parser)
{
  read_digits(&parser->lexer);
  set_current_token(parser, ZEAL_INTEGER_TOKEN);
}

static void lex_token_given_first_char(Parser* parser, const char first)
{
  if (isdigit(first))
  {
    lex_digit(parser);
    return;
  }
  else if (first == '\0')
  {
    set_current_token(parser, ZEAL_EOF_TOKEN);
    return;
  }

  set_current_token(parser, ZEAL_ERROR_TOKEN);
  error(parser, "Lexer error: unexpected character '%c'.\n", first);
}

static void advance(Parser* parser)
{
  eat_leading_whitespace(&parser->lexer);
  lex_token_given_first_char(parser, peek_char(&parser->lexer));
}

static void expect(Parser* parser, TokenType expected)
{
  advance(parser);
  if (parser->current_token.type != expected)
    error(parser, "Parser error: expected %s, found %s.\n",
          token_type_to_string(expected),
          token_type_to_string(parser->current_token.type));
}

void parser_reset_input(Parser* parser, const char* input)
{
  parser->input = input;
  parser->lexer.current_char = input;
  parser->lexer.lexeme_beginning = input;
}

void parse_expression(Parser* parser, Fragment* fragment)
{
  expect(parser, ZEAL_INTEGER_TOKEN);
  generate_integer(parser->compiler, fragment);
  expect(parser, ZEAL_EOF_TOKEN);
}
