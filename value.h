#ifndef zeal_value_h
#define zeal_value_h

#include <stdint.h>

/* The value types currently supported. */
typedef enum
{
  ZEAL_VALUE_NIL,
  ZEAL_VALUE_INTEGER,
} ValueType;

/* Values are just type-tagged ints for now. */
typedef struct
{
  ValueType type;
  int64_t integer;
} Value;

/* Create a value from the given 64-bit int. */
Value value_integer(int64_t integer);

/* Print the given value. */
void value_print(Value value);

/* Print the given value and add a newline character. */
void value_println(Value value);

#endif
