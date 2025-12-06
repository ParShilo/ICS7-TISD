#ifndef ARR_H__
#define ARR_H__

#include "defines.h"

void init_array_queue(array_queue *q);
int is_array_queue_full(array_queue *q);
int is_array_queue_empty(array_queue *q);
int enqueue_array(array_queue *q, request_t request, int printing);
request_t dequeue_array(array_queue *q, int printing);
void print_array_queue(array_queue *q);
int insert_array_element_at_position(array_queue *q, request_t value, int position_from_head);

#endif