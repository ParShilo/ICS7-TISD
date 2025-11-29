#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "array_queue.h"

int init_array_queue(array_queue_t *queue, int max_size)
{
    queue->data = (request_t*)malloc(max_size * sizeof(request_t));
    if (!queue->data) return 0;
    
    queue->pin = 0;
    queue->pout = 0;
    queue->len = 0;
    queue->max_len = max_size;
    queue->in_num = 0;
    queue->total_stay_time = 0;

    return 1;
}

void free_array_queue(array_queue_t *queue)
{
    if (queue->data)
        free(queue->data);
}

int add_to_array_queue(array_queue_t *queue, request_t req) {
    if (queue->len >= queue->max_len) {
        return 0;
    }
    
    queue->data[queue->pin] = req;
    queue->pin = (queue->pin + 1) % queue->max_len;
    queue->len++;
    queue->in_num++;
    return 1;
}

int remove_from_array_queue(array_queue_t *queue, request_t *req) {
    if (queue->len == 0) {
        return 0;
    }
    
    *req = queue->data[queue->pout];
    queue->pout = (queue->pout + 1) % queue->max_len;
    queue->len--;
    return 1;
}

int get_array_queue_length(array_queue_t *queue) {
    return queue->len;
}

void print_array_queue(array_queue_t *queue) {
    if (queue->len == 0) {
        printf("Очередь-массив пуста\n");
        return;
    }
    
    printf("Очередь-массив (длина: %d):\n", queue->len);
    int index = queue->pout;
    for (int i = 0; i < queue->len; i++) {
        printf("Заявка типа %d, время прихода: %.2f\n", 
               queue->data[index].type, queue->data[index].arrival_time);
        index = (index + 1) % queue->max_len;
    }
}