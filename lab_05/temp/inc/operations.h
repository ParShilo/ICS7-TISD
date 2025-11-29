#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "structs.h"

void add_to_array_queue(queue_t *q, int data);
void remove_from_array_queue(queue_t *q);
void print_array_queue(queue_t *q);
void add_to_list_queue(queue_node_t **list_head, int data);
void remove_from_list_queue(queue_node_t **list_head);
void print_list_queue(queue_node_t *list_head);
void demo_array_queue(void);
void demo_list_queue(void);
void clear_list_queue(queue_node_t **list_head);

#endif