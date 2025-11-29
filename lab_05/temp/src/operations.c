#include "operations.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Добавить элемент в очередь-массив
void add_to_array_queue(queue_t *q, int data) 
{
    if (q->len >= q->max_len) 
    {
        printf("Очередь-массив переполнена!\n");
        return;
    }

    // В статическом массиве эта функция может не использоваться напрямую
    printf("Добавление в массив: %d\n", data);
    printf("Текущая длина очереди: %d\n", q->len);
}

// Удалить элемент из очереди-массива
void remove_from_array_queue(queue_t *q) 
{
    if (q->len == 0) 
    {
        printf("Очередь-массив пуста!\n");
        return;
    }

    printf("Удаление из массива\n");
    printf("Текущая длина очереди: %d\n", q->len - 1);
}

// Вывод очереди-массива
void print_array_queue(queue_t *q) 
{
    if (q->len == 0) 
    {
        printf("Очередь-массив пуста\n");
        return;
    }

    printf("=== ОЧЕРЕДЬ-МАССИВ ===\n");
    printf("Длина очереди: %d\n", q->len);
    printf("Всего вошло заявок: %d\n", q->in_num);
    printf("Максимальная длина: %d\n", q->max_len);
    printf("Общее время в очереди: %.2f\n", q->total_stay_time);
}


// Добавить элемент в очередь-список
void add_to_list_queue(queue_node_t **list_head, int data) 
{
    if (list_head == NULL) 
    {
        printf("Ошибка: неверный указатель на список\n");
        return;
    }

    queue_node_t *new_node = (queue_node_t *)malloc(sizeof(queue_node_t));
    if (!new_node) 
    {
        printf("Ошибка выделения памяти для нового элемента\n");
        return;
    }

    new_node->type = data;
    new_node->arrival_time = (double)clock() / CLOCKS_PER_SEC;
    new_node->next = *list_head;
    *list_head = new_node;

    printf("Элемент %d добавлен в очередь-список\n", data);
    printf("Адрес нового элемента: %p\n", (void*)new_node);
}

// Удалить элемент из очереди-списка
void remove_from_list_queue(queue_node_t **list_head) 
{
    if (list_head == NULL || *list_head == NULL) 
    {
        printf("Очередь-список пуста!\n");
        return;
    }

    // Находим предпоследний элемент для правильного удаления из головы (FIFO)
    if ((*list_head)->next == NULL) 
    {
        // В списке только один элемент
        printf("Удаление элемента типа %d из списка\n", (*list_head)->type);
        printf("Адрес удаляемого элемента: %p\n", (void*)(*list_head));
        free(*list_head);
        *list_head = NULL;
    } 
    else 
    {
        // Находим предпоследний элемент
        queue_node_t *current = *list_head;
        while (current->next->next != NULL) 
        {
            current = current->next;
        }
        
        printf("Удаление элемента типа %d из списка\n", current->next->type);
        printf("Адрес удаляемого элемента: %p\n", (void*)current->next);
        free(current->next);
        current->next = NULL;
    }
    
    printf("Элемент успешно удален из очереди-списка\n");
}

// Вывод очереди-списка
void print_list_queue(queue_node_t *list_head) 
{
    if (list_head == NULL) 
    {
        printf("Очередь-список пуста\n");
        return;
    }

    printf("=== ОЧЕРЕДЬ-СПИСОК ===\n");
    queue_node_t *current = list_head;
    int count = 0;
    
    printf("Элементы от хвоста к голове:\n");
    while (current != NULL) 
    {
        printf("[%d] Тип: %d, Время прихода: %.2f, Адрес: %p\n",
               count, current->type, current->arrival_time, (void*)current);
        current = current->next;
        count++;
    }
    printf("Всего элементов: %d\n", count);
}

// Вспомогательная функция для создания тестовой очереди-списка
void create_test_list_queue(queue_node_t **list_head, int count) 
{
    if (list_head == NULL) return;
    
    printf("Создание тестовой очереди-списка из %d элементов...\n", count);
    for (int i = 0; i < count; i++) 
    {
        add_to_list_queue(list_head, i % 2 + 1); // Чередуем типы 1 и 2
    }
    printf("Тестовая очередь-список создана\n");
}

// Вспомогательная функция для полной очистки очереди-списка
void clear_list_queue(queue_node_t **list_head) 
{
    if (list_head == NULL) return;
    
    printf("Очистка очереди-списка...\n");
    while (*list_head != NULL) 
    {
        remove_from_list_queue(list_head);
    }
    printf("Очередь-список очищена\n");
}

// Функция для демонстрации работы с очередью-массивом
void demo_array_queue(void) 
{
    printf("\n=== ДЕМОНСТРАЦИЯ ОЧЕРЕДИ-МАССИВА ===\n");
    
    // Создаем тестовую очередь
    queue_t test_queue = {0};
    
    // Имитируем добавление элементов
    test_queue.len = 3;
    test_queue.in_num = 5;
    test_queue.max_len = 10;
    test_queue.total_stay_time = 25.5;
    
    printf("Состояние очереди-массива:\n");
    print_array_queue(&test_queue);
    
    printf("Добавление элемента...\n");
    add_to_array_queue(&test_queue, 1);
    
    printf("Удаление элемента...\n");
    remove_from_array_queue(&test_queue);
}

// Функция для демонстрации работы с очередью-списком
void demo_list_queue(void) 
{
    printf("\n=== ДЕМОНСТРАЦИЯ ОЧЕРЕДИ-СПИСКА ===\n");
    
    queue_node_t *list = NULL;
    
    // Добавляем несколько элементов
    printf("Добавление элементов в список...\n");
    for (int i = 0; i < 3; i++) 
    {
        add_to_list_queue(&list, i + 1);
    }
    
    // Выводим список
    print_list_queue(list);
    
    // Удаляем один элемент
    printf("Удаление элемента из головы...\n");
    remove_from_list_queue(&list);
    
    // Выводим список после удаления
    print_list_queue(list);
    
    // Очищаем список
    clear_list_queue(&list);
}