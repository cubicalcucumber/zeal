#ifndef zeal_compiler_h
#define zeal_compiler_h

#include <stdbool.h>

#include "parser.h"
#include "../vm/fragment.h"

typedef struct Compiler
{
  Parser* parser;
  bool error;
} Compiler;

void connect_compiler_and_parser(Compiler* compiler, Parser* parser);

/* Generate code and data for the given input string. For now, there exist only
 * a single parse rule: read a number and nothing more.*/
void parse_and_compile(Compiler* compiler, const char* input,
                       Fragment* fragment);

/* Generate code and data for an integer object represented by the current
 * token. */
void generate_integer(Compiler* compiler, Fragment* fragment);

#endif
