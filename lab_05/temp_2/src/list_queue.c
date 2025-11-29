#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "list_queue.h"

void init_list_queue(list_queue_t *queue) {
    queue->pin = NULL;
    queue->pout = NULL;
    queue->len = 0;
    queue->in_num = 0;
    queue->max_len = 0;
    queue->total_stay_time = 0;
}

void free_list_queue(list_queue_t *queue) {
    while (queue->pout != NULL) {
        queue_node_t *temp = queue->pout;
        queue->pout = queue->pout->next;
        free(temp);
    }
    queue->pin = NULL;
    queue->len = 0;
}

int add_to_list_queue(list_queue_t *queue, request_t req, mem_t **mem) {
    queue_node_t *new_node = (queue_node_t*)malloc(sizeof(queue_node_t));
    if (!new_node) return 0;
    
    new_node->data = req;
    new_node->next = NULL;
    
    if (*mem) {
        mem_t *new_mem = (mem_t*)malloc(sizeof(mem_t));
        if (new_mem) {
            new_mem->address = new_node;
            new_mem->busy = 1;
            new_mem->next = *mem;
            *mem = new_mem;
        }
    }
    
    if (queue->pin) {
        queue->pin->next = new_node;
    }
    queue->pin = new_node;
    
    if (!queue->pout) {
        queue->pout = new_node;
    }
    
    queue->len++;
    queue->in_num++;
    if (queue->len > queue->max_len) {
        queue->max_len = queue->len;
    }
    
    return 1;
}

int remove_from_list_queue(list_queue_t *queue, request_t *req, mem_t *mem) {
    if (queue->len == 0) return 0;
    
    queue_node_t *temp = queue->pout;
    *req = temp->data;
    
    queue->pout = queue->pout->next;
    if (!queue->pout) {
        queue->pin = NULL;
    }
    
    if (mem) {
        mem_t *current = mem;
        while (current != NULL) {
            if (current->address == temp) {
                current->busy = 0;
                break;
            }
            current = current->next;
        }
    }
    
    free(temp);
    queue->len--;
    return 1;
}

int get_list_queue_length(list_queue_t *queue) {
    return queue->len;
}

void print_list_queue(list_queue_t *queue) {
    if (queue->len == 0) {
        printf("Очередь-список пуста\n");
        return;
    }
    
    printf("Очередь-список (длина: %d):\n", queue->len);
    queue_node_t *current = queue->pout;
    while (current != NULL) {
        printf("Заявка типа %d, время прихода: %.2f\n", 
               current->data.type, current->data.arrival_time);
        current = current->next;
    }
}