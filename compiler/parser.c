#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <setjmp.h>

#include "compiler.h"
#include "parser.h"

/********************
 * Helper functions *
 ********************/

static const char* token_type_to_string(TokenType type)
{
  if (type == ZEAL_ERROR_TOKEN)
    return "lexer error";
  else if (type == ZEAL_INTEGER_TOKEN)
    return "integer";
  else if (type == ZEAL_PLUS_TOKEN || type == ZEAL_STAR_TOKEN)
    return "binary arithmetic operator";
  else if (type == ZEAL_OPENING_PAREN)
    return "(";
  else if (type == ZEAL_CLOSING_PAREN)
    return ")";
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

/******************
 * Error handling *
 ******************/

/* Used for jumping back to the parsers entry point in the error case. */
static jmp_buf end;

static void display_token_in_context(Parser* parser, Token token)
{
  printf(" | %s\n", parser->input);
  print_n_times(3 + token.beginning - parser->input, ' ');
  /* Also underline current lexer position if lexeme is of length 0. This is
   * useful for error and eof tokens. */
  println_n_times(token.length == 0 ? 1 : token.length, '~');
}

/* Set the error flag, report an error message and abort the parsing process. */
static void error(Parser* parser, Token token, const char* fmt, va_list args)
{
  parser->compiler->error = true;
  vprintf(fmt, args);
  display_token_in_context(parser, token);
  longjmp(end, 0);
}

void error_from_previous_token(Parser* parser, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  error(parser, parser->previous_token, fmt, args);
  va_end(args);
}

void error_from_current_token(Parser* parser, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  error(parser, parser->current_token, fmt, args);
  va_end(args);
}

/*************************************
 * Functions for dealing with tokens *
 *************************************/

static void update_current_token(Parser* parser, TokenType type)
{
  Token token = {.type = type,
                 .beginning = parser->lexer.lexeme_beginning,
                 .length = current_lexeme_length(&parser->lexer)};
  parser->current_token = token;
}

static void lex_integer(Parser* parser)
{
  read_digits(&parser->lexer);
  update_current_token(parser, ZEAL_INTEGER_TOKEN);
}

static void lex_single_char_as(Parser* parser, TokenType token_type)
{
  read_char(&parser->lexer);
  update_current_token(parser, token_type);
}

/* Lex the next token given the first character. */
static void next_token(Parser* parser, const char first)
{
  if (isdigit(first))
  {
    lex_integer(parser);
    return;
  }
  else if (first == '+')
  {
    lex_single_char_as(parser, ZEAL_PLUS_TOKEN);
    return;
  }
  else if (first == '*')
  {
    lex_single_char_as(parser, ZEAL_STAR_TOKEN);
    return;
  }
  else if (first == '(')
  {
    lex_single_char_as(parser, ZEAL_OPENING_PAREN);
    return;
  }
  else if (first == ')')
  {
    lex_single_char_as(parser, ZEAL_CLOSING_PAREN);
    return;
  }
  else if (first == '\0')
  {
    update_current_token(parser, ZEAL_EOF_TOKEN);
    return;
  }

  update_current_token(parser, ZEAL_ERROR_TOKEN);
  error_from_current_token(parser, "Lexer error: unexpected character '%c'.\n",
                           first);
}

/* Mark the current cursor position as beginning of the next token. */
static void mark_lexeme_beginning(Parser* parser)
{
  parser->lexer.lexeme_beginning = parser->lexer.current_char;
}

static void advance(Parser* parser)
{
  parser->previous_token = parser->current_token;
  eat_leading_whitespace(&parser->lexer);
  next_token(parser, peek_char(&parser->lexer));
  mark_lexeme_beginning(parser);
}

static void expect(Parser* parser, TokenType expected)
{
  if (parser->current_token.type == expected)
    return;

  error_from_current_token(parser, "Parser error: expected %s, found %s.\n",
                           token_type_to_string(expected),
                           token_type_to_string(parser->current_token.type));
}

static void consume(Parser* parser, TokenType expected)
{
  expect(parser, expected);
  advance(parser);
}

/***********
 * Parsing *
 ***********/

/* Every token has an associated binding power. */
typedef uint8_t BindingPower;

static BindingPower binding_powers[] = {
    0,  /* ZEAL_ERROR_TOKEN */
    0,  /* ZEAL_INTEGER_TOKEN */
    50, /* ZEAL_PLUS_TOKEN */
    60, /* ZEAL_STAR_TOKEN */
    0,  /* ZEAL_OPENING_PAREN */
    0,  /* ZEAL_CLOSING_PAREN */
    0   /* ZEAL_EOF_TOKEN */
};

void parse_until(Parser* parser, BindingPower binding_power);

void null_integer(Parser* parser) { compile_integer(parser->compiler); }

void null_group(Parser* parser)
{
  parse_until(parser, 0);
  consume(parser, ZEAL_CLOSING_PAREN);
}

/* Null functions are specified for tokens which don't take an expression on
 * the left. */
typedef void (*NullFunction)(Parser*);

static NullFunction null_functions[] = {
    NULL,         /* ZEAL_ERROR_TOKEN */
    null_integer, /* ZEAL_INTEGER_TOKEN */
    NULL,         /* ZEAL_PLUS_TOKEN */
    NULL,         /* ZEAL_STAR_TOKEN */
    null_group,   /* ZEAL_OPENING_PAREN */
    NULL,         /* ZEAL_CLOSING_PAREN */
    NULL          /* ZEAL_EOF_TOKEN */
};

static void left_binary_op(Parser* parser)
{
  Token op = parser->previous_token;
  parse_until(parser, binding_powers[op.type]);
  compile_binary_operator(parser->compiler, op);
}

/* Left functions are specified for tokens which take an expression on the
 * left. */
typedef void (*LeftFunction)(Parser*);

static LeftFunction left_functions[] = {
    NULL,           /* ZEAL_ERROR_TOKEN */
    NULL,           /* ZEAL_INTEGER_TOKEN */
    left_binary_op, /* ZEAL_PLUS_TOKEN */
    left_binary_op, /* ZEAL_STAR_TOKEN */
    NULL,           /* ZEAL_OPENING_PAREN */
    NULL,           /* ZEAL_CLOSING_PAREN */
    NULL            /* ZEAL_EOF_TOKEN */
};

static void try_to_call_null_function(Parser* parser)
{
  NullFunction null_function = null_functions[parser->previous_token.type];

  if (!null_function)
    error_from_previous_token(
        parser, "Parser error: expected expression, found %s.\n",
        token_type_to_string(parser->previous_token.type));

  null_function(parser);
}

static void call_left_function(Parser* parser)
{
  LeftFunction left_function = left_functions[parser->previous_token.type];
  left_function(parser);
}

void parse_until(Parser* parser, BindingPower binding_power)
{
  advance(parser);
  try_to_call_null_function(parser);

  while (binding_power < binding_powers[parser->current_token.type])
  {
    advance(parser);
    call_left_function(parser);
  }
}

void parser_reset_input(Parser* parser, const char* input)
{
  parser->input = input;
  parser->lexer.current_char = input;
  parser->lexer.lexeme_beginning = input;
}

void parse(Parser* parser)
{
  /* In case of an error, the execution will continue here and cause the parser
   * to exit. */
  bool error_detected = setjmp(end);
  if (!error_detected)
  {
    advance(parser);
    parse_until(parser, 0);
    expect(parser, ZEAL_EOF_TOKEN);
  }
}
