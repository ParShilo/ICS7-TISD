#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H

#include "structs.h"

int init_array_queue(array_queue_t *queue, int max_size);
void free_array_queue(array_queue_t *queue);
int add_to_array_queue(array_queue_t *queue, request_t req);
int remove_from_array_queue(array_queue_t *queue, request_t *req);
int get_array_queue_length(array_queue_t *queue);
void print_array_queue(array_queue_t *queue);

#endif