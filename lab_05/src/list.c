#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "errors.h"
#include "list.h"

// Глобальные переменные для отслеживания адресов
node_t *used_addresses[MAX_QUEUE_SIZE * 2];
int address_count = 0;

// Инициализация очереди-списка
void init_list_queue(list_queue *q)
{
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

// Проверка на пустоту
int is_list_queue_empty(list_queue *q)
{
    return q->front == NULL;
}

// Добавление элемента в очередь-список
int enqueue_list(list_queue *q, int value, float arrival_time, int show_address)
{
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node)
    {
        printf("Ошибка выделения памяти!\n");
        return ERROR_MEM;
    }
    
    // Заполнение данных нового узла
    new_node->data = value;
    new_node->arrival_time = arrival_time;
    new_node->next = NULL;
    
    // Сохраняем адрес для отслеживания
    if (address_count < MAX_QUEUE_SIZE * 2)
        used_addresses[address_count++] = new_node;
    
    // Добавление в очередь
    if (is_list_queue_empty(q))
    {
        // Первый элемент в очереди
        q->front = new_node;
        q->rear = new_node;
    }
    else
    {
        // Добавление в конец очереди
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
    
    if (show_address)
        printf("Добавлен элемент %d. Адрес: %p\n", value, (void*)new_node);
    else
        printf("Добавлен элемент %d в очередь-список\n", value);
    
    return ERROR_OK;
}

// Удаление элемента из очереди-списка
int dequeue_list(list_queue *q, int show_address)
{
    if (is_list_queue_empty(q))
    {
        printf("Очередь-список пуста!\n");
        return ERROR_EMPTY;
    }
    
    node_t *temp = q->front;
    int value = temp->data;
    
    if (show_address)
        printf("Удален элемент %d. Адрес: %p\n", value, (void*)temp);
    else
        printf("Удален элемент %d из очереди-списка\n", value);
    
    // Перемещаем указатель на следующий элемент
    q->front = q->front->next;
    
    // Если очередь стала пустой, обнуляем rear
    if (q->front == NULL)
        q->rear = NULL;
    
    // Освобождаем память
    free(temp);
    q->size--;
    
    return value;
}

// Вывод очереди-списка
void print_list_queue(list_queue *q)
{
    if (is_list_queue_empty(q))
    {
        printf("Очередь-список пуста\n");
        return;
    }
    
    printf("Очередь-список (размер: %d): ", q->size);
    node_t *current = q->front;
    
    while (current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Вывод использованных адресов
void print_list_addresses(void)
{
    if (address_count == 0)
    {
        printf("Нет использованных адресов\n");
        return;
    }
    
    printf("Использованные адреса памяти (%d):\n", address_count);
    for (int i = 0; i < address_count; i++)
    {
        printf("%p ", (void*)used_addresses[i]);
        if ((i + 1) % 5 == 0) 
            printf("\n");
    }
    printf("\n");
    
    // Анализ фрагментации
    printf("Анализ фрагментации:\n");
    int fragmented = 0;
    for (int i = 1; i < address_count; i++)
    {
        long diff = (long)used_addresses[i] - (long)used_addresses[i-1];
        if (diff > (long)sizeof(node_t) * 2)
        {
            fragmented = 1;
            printf("Обнаружена фрагментация между %p и %p (разница: %ld байт)\n", 
                   (void*)used_addresses[i-1], (void*)used_addresses[i], diff);
        }
    }
    
    if (!fragmented)
    {
        printf("Значительной фрагментации не обнаружено\n");
    }
}