#ifndef LIST_STACK_H__
#define LIST_STACK_H__

#include "stack.h"

void list_init(ListStack* stack);
int list_is_empty(ListStack* stack);
int list_push(ListStack* stack, int value);
int list_pop(ListStack* stack, int* value);
void list_display(ListStack* stack);
void list_display_with_addresses(ListStack* stack);
void list_free(ListStack* stack);
void list_stack_operations(ListStack* stack);

void add_free_address(void* addr);
void display_free_addresses(void);
void clear_free_addresses(void);

void sort_with_list_stack(void);

#endif