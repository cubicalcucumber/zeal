#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "repl.h"
#include "utils.h"

/* Trim \n, \r, or \r\n from the end of the given string. Return true if the
 * given string could be trimmed. */
static void trim_trailing_newline(char* str)
{
  size_t len = strlen(str);

  if (len > 0 && str[len - 1] == '\n')
    str[len - 1] = '\0';
  else if (len > 0 && str[len - 1] == '\r')
    str[len - 1] = '\0';
  else if (len > 1 && str[len - 1] == '\r' && str[len - 2] == '\n')
    str[len - 2] = '\0';
}

/* TODO: Currently limited to lines of at most 64 characters, fix. */
char* read_line(FILE* file)
{
  assert(file);

  const size_t max_line_length = 64;
  char* line = ZEAL_ALLOC(max_line_length);

  if (!fgets(line, max_line_length, file))
  {
    free(line);
    return NULL;
  }

  trim_trailing_newline(line);

  return line;
}

void display_prompt() { printf(">> "); }
