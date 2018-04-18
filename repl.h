#ifndef zeal_repl_h
#define zeal_repl_h

#include <stdio.h>

/* Read a line from the given file pointer. Note that reading a line stops if
 * the \0 character is encountered. The caller is responsible for freeing the
 * returned string. */
char* read_line(FILE* file);

/* Display the prompt string. */
void display_prompt();

#endif
