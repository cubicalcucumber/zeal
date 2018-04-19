#include <stdio.h>

#include "../utils.h"
#include "value.h"

Value value_integer(int64_t integer)
{
  Value value;
  value.type = ZEAL_VALUE_INTEGER;
  value.integer = integer;
  return value;
}

void value_print(Value value)
{
  if (value.type == ZEAL_VALUE_NIL)
    printf("nil");
  else if (value.type == ZEAL_VALUE_INTEGER)
    printf("%ld : integer", (long) value.integer);
  else
    ZEAL_PANIC("Unknown value type.\n");
}

void value_println(Value value)
{
  value_print(value);
  printf("\n");
}
