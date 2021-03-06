#ifndef zeal_vm_h
#define zeal_vm_h

#include "fragment.h"

/* TODO: Remove limitation of fixed size. */
#define ZEAL_STACK_SIZE 16

typedef struct
{
  Value stack[ZEAL_STACK_SIZE];
  Fragment* fragment;
} VirtualMachine;

/* Initialize the virtual machine. */
void vm_init(VirtualMachine* vm);

/* Run the given program fragment. */
void run(VirtualMachine* vm, Fragment* fragment);

#endif
