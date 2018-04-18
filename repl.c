#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "repl.h"
#include "utils.h"

/* Trim \n, \r, or \r\n from the end of the given string. Return true if the
 * given string could be trimmed. */
static bool trim_trailing_newline(char* str)
{
  size_t len = strlen(str);

  if (len > 0 && str[len - 1] == '\n')
  {
    str[len - 1] = '\0';
    return true;
  }
  else if (len > 0 && str[len - 1] == '\r')
  {
    str[len - 1] = '\0';
    return true;
  }
  else if (len > 1 && str[len - 1] == '\r' && str[len - 2] == '\n')
  {
    str[len - 2] = '\0';
    return true;
  }
  return false;
}

char* read_line(FILE* file)
{
  assert(file);

  const size_t init_size = 64;
  size_t current_size = init_size;

  char* line = ZEAL_ALLOC(current_size);

  while (fgets(line + current_size - init_size, init_size, file))
  {
    /* If we don't find a newline character in the string we read, this will be
     * the new size of the memory we allocated. */
    current_size += init_size - 1;

    bool found_newline_char = trim_trailing_newline(line);
    if (found_newline_char)
      break;

    line = ZEAL_REALLOC(line, current_size);
  }

  /* If we have never read anything, return null. */
  if (current_size == init_size)
  {
    free(line);
    return NULL;
  }

  return line;
}

void display_prompt() { printf(">> "); }
