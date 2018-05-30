#ifndef zeal_lexer_h
#define zeal_lexer_h

#include <stddef.h>

/* The lexer works on a string somewhere else in memory. It views one character
 * at a time and remembers the beginning of the lexeme currently being read. */
typedef struct
{
  const char* current_char;
  const char* lexeme_beginning;
} Lexer;

/* Read the current character as part of the current lexeme. */
void read_char(Lexer* lexer);

/* Peek the character currently being viewed. */
char peek_char(Lexer* lexer);

/* Read a sequence of digit characters as part of the current lexeme. */
void read_digits(Lexer* lexer);

/* Jump over leading whitespace in the current input. */
void eat_leading_whitespace(Lexer* lexer);

/* Return the size of the lexeme currently being lexed. */
size_t current_lexeme_length(Lexer* lexer);

#endif
