#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "repl.h"
#include "utils.h"

static void display_prompt() { printf(">> "); }

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

/* Read a line from the given file pointer. Note that reading a line stops if
 * the \0 character is encountered. The caller is responsible for freeing the
 * returned string. */
static char* read_line(FILE* file)
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

static bool is_empty_line(Repl* repl) {
  for (size_t i = 0; i < strlen(repl->line); ++i)
    if (!is_whitespace(repl->line[i]))
      return false;
  return true;
}

static void catch_exit_command(Repl* repl)
{
  if (strcmp(repl->line, ":q") == 0)
    repl->keep_running = false;
}

static void read_eval_print(Repl* repl)
{
  display_prompt();
  repl->line = read_line(stdin);
  catch_exit_command(repl);
  if (!is_empty_line(repl) && repl->keep_running)
    evaluate(&repl->interpreter, repl->line);
  free(repl->line);
}

void repl_init(Repl* repl)
{
  interpreter_init(&repl->interpreter);
  repl->keep_running = true;
  repl->line = NULL;
}

void repl_run(Repl* repl)
{
  while (repl->keep_running)
    read_eval_print(repl);
}
