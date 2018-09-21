#include <stdio.h>

#include "../utils.h"
#include "value.h"

Value value_nil() { return (Value){.type = ZEAL_VALUE_NIL, .integer = 0}; }

Value value_from_integer(int64_t integer)
{
  return (Value){.type = ZEAL_VALUE_INTEGER, .integer = integer};
}

void value_print(Value value)
{
  if (value.type == ZEAL_VALUE_NIL)
    printf("nil");
  else if (value.type == ZEAL_VALUE_INTEGER)
    printf("%lld : integer", value.integer);
  else
    ZEAL_PANIC("Unknown value type.\n");
}

void value_println(Value value)
{
  value_print(value);
  printf("\n");
}
