#ifndef zeal_fragment_h
#define zeal_fragment_h

#include <stdint.h>

#include "value.h"

/* The instruction layout is not yet fully developed. For now we will use the
 * following layout:
 *  ___________________________________
 * |   A    |   B    |  DST   |   OP   |
 *  -----------------------------------
 *  ^        ^        ^        ^      ^
 *  0        8        16       24     31
 */
typedef uint32_t Instruction;

/* The opcodes currently supported by the virtual machine. */
typedef enum
{
  /* Load a value from the constant pool at index A into stack slot DST. */
  ZEAL_OP_LOAD,
  /* Print the stack value in slot A. */
  ZEAL_OP_PRINT,
  /* Halt the execution. */
  ZEAL_OP_HALT
} OpCode;

/* TODO: Remove limitation of fixed sizes for the code and data buffers. */
#define ZEAL_CODE_BUFFER_SIZE 16
#define ZEAL_DATA_BUFFER_SIZE 16

typedef struct
{
  size_t size;
  Instruction instructions[ZEAL_CODE_BUFFER_SIZE];
} CodeBuffer;

typedef struct
{
  size_t size;
  Value values[ZEAL_DATA_BUFFER_SIZE];
} DataBuffer;

/* A fragment of a program consisting of compiled byte code and associated
 * data. */
typedef struct
{
  DataBuffer data;
  CodeBuffer code;
} Fragment;

void fragment_init(Fragment* fragment);

size_t fragment_add_code(Fragment* fragment, Instruction instruction);

size_t fragment_add_data(Fragment* fragment, Value value);

void debug(Fragment* fragment);

#endif
