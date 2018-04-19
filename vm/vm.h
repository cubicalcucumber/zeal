#ifndef zeal_vm_h
#define zeal_vm_h

#include "fragment.h"

/* TODO: Remove limitation of fixed size. */
#define ZEAL_STACK_SIZE 3

typedef struct
{
  Value stack[ZEAL_STACK_SIZE];
  Fragment* fragment;
} VirtualMachine;

/* Run the given program fragment. */
void run(VirtualMachine* vm, Fragment* fragment);

#endif
