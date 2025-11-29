#ifndef LIST_H
#define LIST_H

#include "structs.h"
#include <stdbool.h>

unsigned long simulate_list(mem_t **mem, int *mem_used, bool need_input);
void show_mem(mem_t **mem);

#endif