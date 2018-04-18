#include <ctype.h>
#include <stdbool.h>

#include "lexer.h"

/* Return true if the given character is a whitespace character. */
static bool is_whitespace(char c) { return c == ' ' || c == '\t'; }

/* Jump over the current character. */
static void eat_char(Lexer* lexer)
{
  lexer->current_char++;
  lexer->token_beginning = lexer->current_char;
}

void lexer_set_input(Lexer* lexer, const char* input)
{
  lexer->current_char = input;
  lexer->token_beginning = input;
}

char peek_char(Lexer* lexer) { return *lexer->current_char; }

void read_char(Lexer* lexer) { lexer->current_char++; }

void read_digits(Lexer* lexer)
{
  while (isdigit(peek_char(lexer)))
    read_char(lexer);
}

void eat_leading_whitespace(Lexer* lexer)
{
  while (is_whitespace(peek_char(lexer)))
    eat_char(lexer);
}
