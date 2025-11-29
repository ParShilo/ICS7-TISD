#include <stdio.h>
#include "defines.h"
#include "errors.h"
#include "arr.h"

// Инициализация очереди-массива
void init_array_queue(array_queue *q)
{
    q->pin = 0;
    q->pout = 0;
    q->size = 0;
}

// Проверка на полноту
int is_array_queue_full(array_queue *q)
{
    return q->size == MAX_QUEUE_SIZE;
}

// Проверка на пустоту
int is_array_queue_empty(array_queue *q)
{
    return q->size == 0;
}

// Добавление элемента в очередь-массив
int enqueue_array(array_queue *q, request_t request, int printing)
{
    if (is_array_queue_full(q))
        return ERROR_OVEFLOW;
    
    q->data[q->pin] = request;
    q->pin = (q->pin + 1) % MAX_QUEUE_SIZE;
    q->size++;
    
    if (printing)
        printf("Добавлен элемент %d в очередь-массив. Позиция: %d\n", 
               request.type, (q->pin - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE);

    return ERROR_OK;
}

// Удаление элемента из очереди-массива
request_t dequeue_array(array_queue *q, int printing)
{
    request_t empty_request = {0, 0.0};
    
    if (is_array_queue_empty(q))
        return empty_request;
    
    request_t value = q->data[q->pout];
    
    if (printing)
        printf("Удален элемент %d из очереди-массива. Позиция: %d\n", value.type, q->pout);
    
    q->pout = (q->pout + 1) % MAX_QUEUE_SIZE;
    q->size--;

    return value;
}

// Вывод очереди-массива
void print_array_queue(array_queue *q)
{
    if (is_array_queue_empty(q))
    {
        printf("Очередь-массив пуста\n");
        return;
    }
    
    printf("Очередь-массив (размер: %d): ", q->size);
    int i = q->pout;
    int count = 0;
    
    while (count < q->size)
    {
        printf("%d ", q->data[i].type);
        i = (i + 1) % MAX_QUEUE_SIZE;
        count++;
    }
    printf("\n");
}