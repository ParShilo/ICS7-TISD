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
    q->pout = NULL;
    q->pin = NULL;
    q->size = 0;
}

// Проверка на пустоту
int is_list_queue_empty(list_queue *q)
{
    return q->pout == NULL;
}

// Добавление элемента в очередь-список
int enqueue_list(list_queue *q, request_t req, int show_address)
{
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node)
        return ERROR_MEM;
    
    new_node->request = req;
    new_node->next = NULL;
    
    if (address_count < MAX_QUEUE_SIZE * 2)
        used_addresses[address_count++] = new_node;
    
    if (is_list_queue_empty(q))
    {
        q->pout = new_node;
        q->pin = new_node;
    }
    else
    {
        q->pin->next = new_node;
        q->pin = new_node;
    }
    q->size++;
    
    if (show_address)
        printf("Добавлен элемент типа %d. Адрес: %p\n", req.type, (void*)new_node);

    return ERROR_OK;
}

// Удаление элемента из очереди-списка
request_t dequeue_list(list_queue *q, int show_address)
{
    request_t empty = {0, 0.0};
    if (is_list_queue_empty(q))
        return empty;
    
    node_t *temp = q->pout;
    request_t req = temp->request;
    
    if (show_address)
        printf("Удален элемент типа %d. Адрес: %p\n", req.type, (void*)temp);
    
    q->pout = q->pout->next;
    if (q->pout == NULL)
        q->pin = NULL;
    
    free(temp);
    q->size--;
    
    return req;
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
    node_t *current = q->pout;
    while (current != NULL)
    {
        printf("%d ", current->request.type);
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

// Функция вставки заявки на заданную позицию
int insert_list_element_at_position(list_queue *q, request_t req, int position)
{
    if (q->size >= MAX_QUEUE_SIZE)
        return ERROR_OVEFLOW;

    if (position > q->size)
        position = q->size;
    
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node)
        return ERROR_MEM;
    
    new_node->request = req;
    node_t *current = NULL;

    if (position == 0)
    {
        new_node->next = q->pout;
        q->pout = new_node;
        if (q->pin == NULL)
            q->pin = new_node;
    }
    else if (position == q->size)
    {
        new_node->next = NULL;
        q->pin->next = new_node;
        q->pin = new_node;
    }
    else
    {
        current = q->pout;
        for (int i = 0; i < position - 1; i++)
            current = current->next;
        
        new_node->next = current->next;
        current->next = new_node;
    }
    
    q->size++;
    
    return ERROR_OK;
}