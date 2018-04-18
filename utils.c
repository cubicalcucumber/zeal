#include "utils.h"

void* zeal_alloc(const size_t size, const char* file, const uint32_t line)
{
  void* ptr = malloc(size);
  if (!ptr)
    ZEAL_PANIC("Allocation failed in %s (line %d).\n", file, line);
  return ptr;
}
