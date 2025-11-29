#ifndef SIMULATION_H
#define SIMULATION_H

#include "structs.h"
#include <stdbool.h>

void simulate_array_queue(int *mem_used, bool show_stats);
void simulate_list_queue(int *mem_used, mem_t **mem, bool show_stats);
void show_memory_addresses(mem_t *mem);
void compare_queues();

#endif