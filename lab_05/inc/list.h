#ifndef LIST_H__
#define LIST_H__

void init_list_queue(list_queue *q);
int is_list_queue_empty(list_queue *q);
int enqueue_list(list_queue *q, request_t req, int show_address);
request_t dequeue_list(list_queue *q, int show_address);
void print_list_queue(list_queue *q);
void print_list_addresses(void);
int insert_list_element_at_position(list_queue *q, request_t req, int position);

#endif