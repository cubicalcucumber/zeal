#include "utils.h"

void* zeal_alloc(const size_t size, const char* file, const uint32_t line)
{
  void* ptr = malloc(size);
  if (!ptr)
    ZEAL_PANIC("Allocation failed in %s (line %d).\n", file, line);
  return ptr;
}

void* zeal_realloc(void* ptr, const size_t size, const char* file,
                   const uint32_t line)
{
  if (size == 0)
  {
    free(ptr);
    return NULL;
  }

  void* new_ptr = realloc(ptr, size);
  if (!new_ptr)
    ZEAL_PANIC("(Re-)Allocation failed in %s (line %d).\n", file, line);
  return new_ptr;
}

bool is_whitespace(char c) { return c == ' ' || c == '\t'; }
