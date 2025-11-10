#ifndef ARRAY_STACK_H__
#define ARRAY_STACK_H__

#include "stack.h"

void array_init(ArrayStack* stack);
int array_is_empty(ArrayStack* stack);
int array_is_full(ArrayStack* stack);
int array_push(ArrayStack* stack, int value);
int array_pop(ArrayStack* stack, int* value);
void array_display(ArrayStack* stack);
void array_stack_operations(ArrayStack* stack);
void sort_with_array_stack(void);

#endif