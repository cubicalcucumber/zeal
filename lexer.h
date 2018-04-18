#ifndef zeal_lexer_h
#define zeal_lexer_h

/* The lexer works on a string somewhere else in memory. It views one character
 * at a time and remembers the beginning of the token currently being read. */
typedef struct
{
  const char* current_char;
  const char* token_beginning;
} Lexer;

/* Set the current input string. */
void lexer_set_input(Lexer* lexer, const char* input);

/* Peek the character currently being viewed. */
char peek_char(Lexer* lexer);

/* Read the current character as part of the current token. */
void read_char(Lexer* lexer);

/* Read a sequence of digit characters as part of the current token. */
void read_digits(Lexer* lexer);

/* Jump over leading whitespace in the current input. */
void eat_leading_whitespace(Lexer* lexer);

#endif
