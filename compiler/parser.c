#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <setjmp.h>

#include "compiler.h"
#include "parser.h"

/* Used for jumping back to the parsers entry point in the error case. */
static jmp_buf end;

static const char* token_type_to_string(TokenType type)
{
  if (type == ZEAL_ERROR_TOKEN)
    return "lexer error";
  else if (type == ZEAL_INTEGER_TOKEN)
    return "integer";
  else if (type == ZEAL_PLUS_TOKEN || type == ZEAL_STAR_TOKEN)
    return "binary arithmetic operator";
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

static void display_token_in_context(Parser* parser, Token token)
{
  printf(" | %s\n", parser->input);
  print_n_times(3 + token.beginning - parser->input, ' ');
  /* Also underline current lexer position if lexeme is of length 0. This is
   * useful for error and eof tokens. */
  println_n_times(token.length == 0 ? 1 : token.length, '~');
}

static void mark_lexeme_beginning(Parser* parser)
{
  parser->lexer.lexeme_beginning = parser->lexer.current_char;
}

static void set_current_token(Parser* parser, TokenType type)
{
  Token token = {.type = type,
                 .beginning = parser->lexer.lexeme_beginning,
                 .length = current_lexeme_length(&parser->lexer)};
  parser->current_token = token;
  mark_lexeme_beginning(parser);
}

void error_from_token(Parser* parser, Token token, const char* fmt,
                      va_list args)
{
  if (parser->compiler->error)
    return;
  parser->compiler->error = true;
  vprintf(fmt, args);
  display_token_in_context(parser, token);
  longjmp(end, 0);
}

void error_from_previous_token(Parser* parser, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  error_from_token(parser, parser->previous_token, fmt, args);
  va_end(args);
}

void error(Parser* parser, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  error_from_token(parser, parser->current_token, fmt, args);
  va_end(args);
}

static void lex_integer(Parser* parser)
{
  read_digits(&parser->lexer);
  set_current_token(parser, ZEAL_INTEGER_TOKEN);
}

static void next_token(Parser* parser, const char first)
{
  if (isdigit(first))
  {
    lex_integer(parser);
    return;
  }
  else if (first == '+')
  {
    read_char(&parser->lexer);
    set_current_token(parser, ZEAL_PLUS_TOKEN);
    return;
  }
  else if (first == '*')
  {
    read_char(&parser->lexer);
    set_current_token(parser, ZEAL_STAR_TOKEN);
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
  parser->previous_token = parser->current_token;
  eat_leading_whitespace(&parser->lexer);
  next_token(parser, peek_char(&parser->lexer));
}

static void expect(Parser* parser, TokenType expected)
{
  if (parser->current_token.type == expected)
    return;

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

typedef uint8_t BindingPower;

static BindingPower binding_powers[] = {
    0,  /* ZEAL_ERROR_TOKEN */
    20, /* ZEAL_INTEGER_TOKEN */
    50, /* ZEAL_PLUS_TOKEN */
    60, /* ZEAL_STAR_TOKEN */
    0   /* ZEAL_EOF_TOKEN */
};

void parse_until(Parser* parser, Fragment* fragment,
                 BindingPower binding_power);

static void parse_infix(Parser* parser, Fragment* fragment)
{
  Token op = parser->previous_token;
  parse_until(parser, fragment, binding_powers[op.type]);
  generate_binary_op(parser->compiler, op, fragment);
}

void parse_until(Parser* parser, Fragment* fragment, BindingPower binding_power)
{
  expect(parser, ZEAL_INTEGER_TOKEN);
  advance(parser);
  generate_integer(parser->compiler, fragment);

  while (binding_power < binding_powers[parser->current_token.type])
  {
    advance(parser);
    expect(parser, ZEAL_INTEGER_TOKEN);
    parse_infix(parser, fragment);
  }
}

void parse(Parser* parser, Fragment* fragment)
{
  /* In case of an error, the execution will continue here and cause the parser
   * to exit. */
  bool error_detected = setjmp(end);
  if (!error_detected)
  {
    advance(parser);
    parse_until(parser, fragment, 0);
    expect(parser, ZEAL_EOF_TOKEN);
  }
}
