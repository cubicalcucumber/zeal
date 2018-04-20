#ifndef zeal_compiler_h
#define zeal_compiler_h

#include <stdbool.h>

#include "../vm/fragment.h"
#include "parser.h"

typedef struct
{
  Parser parser;
  bool error;
} Compiler;

/* Reset the given compiler instance. */
void compiler_reset(Compiler* compiler);

/* Parse the given input string. For now, there exist only a single parse rule:
 * read a number and nothing more.*/
void compile_expression(Compiler* compiler, const char* input,
                        Fragment* fragment);

#endif
