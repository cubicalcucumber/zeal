#ifndef zeal_token_h
#define zeal_token_h

#include <stdint.h>

/* Possible token types used by the lexer. */
typedef enum
{
  ZEAL_INTEGER_TOKEN,
  ZEAL_EOF_TOKEN,
} TokenType;

/* Each token consists of a type and a reference to the lexeme which lies
 * somewhere else in memory. */
typedef struct
{
  TokenType type;
  const char* beginning;
  size_t length;
} Token;

#endif