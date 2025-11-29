#ifndef LIST_STACK_H__
#define LIST_STACK_H__

#include "stack.h"

// Структура для узла списка освобожденных адресов
typedef struct FreeNode 
{
    void* freed_addr;
    struct FreeNode* next;
} FreeNode;

extern FreeNode* free_addresses;
extern int free_count;

void list_init(ListStack* stack);
int list_is_empty(ListStack* stack);
int list_is_full(ListStack* stack);
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