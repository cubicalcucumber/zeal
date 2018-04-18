#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "repl.h"

int32_t main()
{
  char* line = NULL;
  bool is_running = true;

  /* The REPL loop. */
  do
  {
    display_prompt();
    line = read_line(stdin);

    if (strcmp(line, ":q") == 0)
      is_running = false;
    else
      printf("> read line: %s\n", line);

    free(line);
  } while(is_running);

  return 0;
}
