#ifndef zeal_compiler_h
#define zeal_compiler_h

#include "fragment.h"
#include "parser.h"

typedef struct
{
  Parser parser;
} Compiler;

typedef enum
{
  ZEAL_COMPILE_OK,
  ZEAL_COMPILE_ERROR
} CompileResult;

/* Parse the given input string. For now, there exist only a single parse rule:
 * read a number and nothing more.*/
CompileResult compile_number(Compiler* compiler, const char* input,
                             Fragment* fragment);

#endif
