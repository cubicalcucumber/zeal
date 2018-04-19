#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "utils.h"
#include "value.h"
#include "vm.h"

/* Setup the program fragment to be executed. */
static void setup_fragment(VirtualMachine* vm, Fragment* fragment)
{
  vm->fragment = fragment;
}

void run(VirtualMachine* vm, Fragment* fragment)
{
  setup_fragment(vm, fragment);

  Value* sp = &vm->stack[ZEAL_STACK_SIZE - 1];
  Instruction* ip = fragment->code.instructions;
  Instruction instruction;
  OpCode op;

  while (true)
  {
    /* Fetch next instruction and decode operand. */
    instruction = *ip++;
    op = instruction & 0xff;

    switch(op)
    {
      case ZEAL_OP_LOAD:
      {
        uint8_t dst = (instruction & 0xff00) >> 8;
        uint8_t src = (instruction & 0xff0000) >> 16;
        *(sp - dst) = fragment->data.values[src];
        break;
      }
      case ZEAL_OP_PRINT:
      {
        uint8_t src = (instruction & 0xff0000) >> 16;
        value_println(*(sp - src));
        break;
      }
      case ZEAL_OP_HALT:
      {
        return;
      }
    }
  }
}
