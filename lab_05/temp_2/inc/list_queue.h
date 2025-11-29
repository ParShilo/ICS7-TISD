#ifndef LIST_QUEUE_H
#define LIST_QUEUE_H

#include "structs.h"

void init_list_queue(list_queue_t *queue);
void free_list_queue(list_queue_t *queue);
int add_to_list_queue(list_queue_t *queue, request_t req, mem_t **mem);
int remove_from_list_queue(list_queue_t *queue, request_t *req, mem_t *mem);
int get_list_queue_length(list_queue_t *queue);
void print_list_queue(list_queue_t *queue);

#endif