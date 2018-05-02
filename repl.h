#ifndef zeal_repl_h
#define zeal_repl_h

#include <stdio.h>

#include "zeal.h"

typedef struct
{
  Interpreter interpreter;
  bool keep_running;
  char* line;
} Repl;

void repl_init(Repl* repl);

void repl_run(Repl* repl);

#endif
