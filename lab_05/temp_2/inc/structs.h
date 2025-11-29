#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

typedef struct
{
    int type; // 1 - заявка 1 типа, 2 - заявка 2 типа
    double arrival_time;
    int process_count; // для заявки 2 типа - счетчик обработок
} request_t;

typedef struct queue_node_t
{
    request_t data;
    struct queue_node_t *next;
} queue_node_t;

typedef struct
{
    queue_node_t *pin;
    queue_node_t *pout;
    int len;
    int in_num;
    int max_len;
    double total_stay_time;
} list_queue_t;

typedef struct
{
    request_t *data;
    int pin;
    int pout;
    int len;
    int max_len;
    int in_num;
    double total_stay_time;
} array_queue_t;

typedef struct
{
    double current_time;
    double downtime;
    int processed_count_1;
    int processed_count_2;
    int triggering;
} oa_t;

typedef struct mem_t
{
    void *address;
    int busy;
    struct mem_t *next;
} mem_t;

uint64_t tick(void);
double get_time(int start, int end);

#endif