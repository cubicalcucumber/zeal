#ifndef zeal_utils_h
#define zeal_utils_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Print an error message to stderr and exit the program. */
#define ZEAL_PANIC(...)                                                        \
  do                                                                           \
  {                                                                            \
    fprintf(stderr, "Panicking in %s (line %d): ", __FILE__, __LINE__);        \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);                                                                   \
  } while (0);

/* Try to allocate `size` bytes of memory and panic if the allocation fails. */
#define ZEAL_ALLOC(size) zeal_alloc(size, __FILE__, __LINE__);

/* Wrapper around malloc(). Panic if the allocation fails. */
void* zeal_alloc(const size_t size, const char* file, const uint32_t line);

#endif
