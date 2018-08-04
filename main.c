#include <stdint.h>

#include "repl.h"

int32_t main()
{
  Repl repl;
  repl_init(&repl);
  repl_run(&repl);
  return 0;
}
