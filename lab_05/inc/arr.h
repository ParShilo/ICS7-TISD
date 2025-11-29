#ifndef ARR_H__
#define ARR_H__

#include "defines.h"
#include "errors.h"

// Структура для заявки с временем поступления
typedef struct
{
    int type;           // тип заявки (1 или 2)
    double arrival_time; // время поступления в систему
} request_t;

// Структура для очереди на массиве
typedef struct
{
    request_t data[MAX_QUEUE_SIZE];  // Заявки
    int pin;    // указатель на вставку (rear)
    int pout;   // указатель на извлечение (front)
    int size;   // текущий размер
} array_queue;

// Прототипы функций
void init_array_queue(array_queue *q);
int is_array_queue_full(array_queue *q);
int is_array_queue_empty(array_queue *q);
int enqueue_array(array_queue *q, request_t request, int printing);
request_t dequeue_array(array_queue *q, int printing);
void print_array_queue(array_queue *q);

#endif