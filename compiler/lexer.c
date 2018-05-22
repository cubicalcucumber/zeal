#include <ctype.h>
#include <stdbool.h>
#include <stdbool.h>

#include "lexer.h"
#include "../utils.h"

/* Read the current character as part of the current lexeme. */
static void read_char(Lexer* lexer) { lexer->current_char++; }

/* Jump over the current character. */
static void eat_char(Lexer* lexer)
{
  lexer->current_char++;
  lexer->lexeme_beginning = lexer->current_char;
}

char peek_char(Lexer* lexer) { return *lexer->current_char; }

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

size_t current_lexeme_length(Lexer* lexer)
{
  return lexer->current_char - lexer->lexeme_beginning;
}
