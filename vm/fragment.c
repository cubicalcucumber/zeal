#include "fragment.h"
#include "../utils.h"

void fragment_init(Fragment* fragment)
{
  fragment->code.size = 0;
  fragment->data.size = 0;
}

size_t fragment_add_code(Fragment* fragment, Instruction instruction)
{
  if (fragment->code.size >= ZEAL_CODE_BUFFER_SIZE)
    ZEAL_PANIC("Code buffer limit of %d reached.\n", ZEAL_CODE_BUFFER_SIZE);
  fragment->code.instructions[fragment->code.size] = instruction;
  return fragment->code.size++;
}

size_t fragment_add_data(Fragment* fragment, Value value)
{
  if (fragment->data.size >= ZEAL_DATA_BUFFER_SIZE)
    ZEAL_PANIC("Data buffer limit of %d reached.\n", ZEAL_DATA_BUFFER_SIZE);
  fragment->data.values[fragment->data.size] = value;
  return fragment->data.size++;
}
