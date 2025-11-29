#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "defines.h"
#include "errors.h"
#include "arr.h"
#include "simulate.h"

// Определения переменных
double q1_arrival_min = 0, q1_arrival_max = 5;
double q1_service_min = 0, q1_service_max = 4;
double q2_service_min = 0, q2_service_max = 4;

// Генерация случайного времени в диапазоне [min, max]
double generate_random_time(double min, double max)
{
    return min + (max - min) * (rand() / (double)RAND_MAX);
}

// Нахождение элемента на позиции от головы (для заявки 2 типа)
int find_element_at_position(array_queue *q, int position_from_head, request_t *value)
{
    if (is_array_queue_empty(q) || position_from_head >= q->size)
        return ERROR_EMPTY;
    
    int index = (q->pout + position_from_head) % MAX_QUEUE_SIZE;
    *value = q->data[index];
    return ERROR_OK;
}

// Удаление элемента на позиции от головы
int remove_element_at_position(array_queue *q, int position_from_head)
{
    if (is_array_queue_empty(q) || position_from_head >= q->size)
        return ERROR_EMPTY;
    
    // Если удаляем первый элемент - используем стандартную функцию
    if (position_from_head == 0)
    {
        dequeue_array(q, 0);
        return ERROR_OK;
    }
    
    // Сдвигаем элементы
    int queue_size = q->size;
    
    // Сдвигаем все элементы после удаляемого
    for (int i = position_from_head; i < queue_size - 1; i++)
    {
        int current_index = (q->pout + i) % MAX_QUEUE_SIZE;
        int next_index = (q->pout + i + 1) % MAX_QUEUE_SIZE;
        q->data[current_index] = q->data[next_index];
    }
    
    q->pin = (q->pin - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    q->size--;
    
    return ERROR_OK;
}

// Вставка элемента на позицию от головы
int insert_element_at_position(array_queue *q, request_t value, int position_from_head)
{
    if (is_array_queue_full(q))
        return ERROR_OVEFLOW;
    
    if (position_from_head > q->size)
        position_from_head = q->size; // В конец, если позиция больше размера
    
    // Сдвигаем элементы чтобы освободить место
    for (int i = q->size; i > position_from_head; i--)
    {
        int current_index = (q->pout + i) % MAX_QUEUE_SIZE;
        int prev_index = (q->pout + i - 1) % MAX_QUEUE_SIZE;
        q->data[current_index] = q->data[prev_index];
    }
    
    // Вставляем новый элемент
    int insert_index = (q->pout + position_from_head) % MAX_QUEUE_SIZE;
    q->data[insert_index] = value;
    
    q->pin = (q->pin + 1) % MAX_QUEUE_SIZE;
    q->size++;
    
    return ERROR_OK;
}

// Основная функция моделирования для массива
int simulate_array_queue(int printing)
{
    array_queue queue;
    system_state_t state;
    statistics_t stats;
    
    double next_event_time, service_time;
    int event_type, insert_position, rc = ERROR_OK;
    request_t next_request, type2_request;

    // Инициализация
    init_array_queue(&queue);
    srand(time(NULL));
    
    // Начальное состояние системы
    state.time = 0.0;
    state.next_arrival_type1 = generate_random_time(q1_arrival_min, q1_arrival_max);
    state.next_service_end = 0.0;
    state.device_busy = 0;
    state.current_request.type = 0;
    state.current_request.arrival_time = 0.0;
    state.service_start_time = 0.0;
    
    // Начальная статистика
    stats.total_requests_type1 = 0;
    stats.served_requests_type1 = 0;
    stats.total_requests_type2 = 0;
    stats.total_queue_time = 0.0;
    stats.total_modeling_time = 0.0;
    stats.device_idle_time = 0.0;
    stats.max_queue_length = 0;
    stats.total_queue_length = 0.0;
    stats.measurements_count = 0;
    
    if (printing)
    {
        printf("=== МОДЕЛИРОВАНИЕ СИСТЕМЫ МАССОВОГО ОБСЛУЖИВАНИЯ (МАССИВ) ===\n");
        printf("Параметры системы:\n");
        printf("  Заявки 1 типа: приход [%.1f, %.1f], обслуживание [%.1f, %.1f]\n", q1_arrival_min, q1_arrival_max, q1_service_min, q1_service_max);
        printf("  Заявки 2 типа: обслуживание [%.1f, %.1f]\n", q2_service_min, q2_service_max);
        printf("Начало моделирования...\n\n");
    }
    
    // В начале процесса заявка 2-го типа входит в ОА
    state.device_busy = 1;
    state.current_request.type = REQUEST_TYPE_2;
    state.current_request.arrival_time = 0.0;
    state.service_start_time = 0.0;
    state.next_service_end = generate_random_time(q2_service_min, q2_service_max);
    stats.total_requests_type2++; // <-- Первая заявка 2-го типа

    // Основной цикл моделирования
    while (stats.served_requests_type1 < 1000)
    {
        // Определяем следующее событие
        next_event_time = state.next_arrival_type1;
        event_type = 1; // 1 - приход заявки 1 типа, 2 - окончание обслуживания
        
        if (state.device_busy && state.next_service_end < next_event_time)
        {
            next_event_time = state.next_service_end;
            event_type = 2;
        }
        
        // Обновляем статистику длины очереди
        stats.total_queue_length += queue.size;
        stats.measurements_count++;
        if (queue.size > stats.max_queue_length)
            stats.max_queue_length = queue.size;
        
        // Обновляем время простоя аппарата
        if (!state.device_busy)
            stats.device_idle_time += (next_event_time - state.time);
        
        state.time = next_event_time;
        
        if (event_type == 1)  
        {
            if (is_array_queue_full(&queue))
            {
                if (printing)
                    printf("Время %.2f: ОЧЕРЕДЬ ПЕРЕПОЛНЕНА!\n", state.time);
                return ERROR_OVEFLOW;
            }

            // Создаем заявку 1 типа
            request_t new_request = {REQUEST_TYPE_1, state.time};
            
            // Добавляем заявку в очередь
            if (enqueue_array(&queue, new_request, printing) == ERROR_OK)
                stats.total_requests_type1++;
            
            // Планируем следующий приход
            state.next_arrival_type1 = state.time + generate_random_time(q1_arrival_min, q1_arrival_max);
        }
        else // Окончание обслуживания
        {
            if (printing)
                printf("Время %.2f: Завершено обслуживание заявки типа %d\n", state.time, state.current_request.type);
            
            if (state.current_request.type == REQUEST_TYPE_1)
            {
                stats.served_requests_type1++;
                stats.total_queue_time += (state.service_start_time - state.current_request.arrival_time);
                
                if (stats.served_requests_type1 % 100 == 0 && printing)
                    print_intermediate_stats(&stats, queue.size, stats.served_requests_type1);
            }
            else // REQUEST_TYPE_2
            {
                // Возвращаем ту же заявку 2-го типа в очередь
                type2_request = state.current_request; // <-- Копируем ту же заявку
                type2_request.arrival_time = state.time; // <-- Обновляем время поступления
                
                insert_position = (queue.size >= 3) ? 3 : queue.size;
                rc = insert_element_at_position(&queue, type2_request, insert_position);
                if (rc != ERROR_OK)
                    return rc;
            }
            
            // Начинаем обслуживание следующей заявки, если есть
            if (!is_array_queue_empty(&queue))
            {
                next_request = dequeue_array(&queue, printing);
                
                state.device_busy = 1;
                state.current_request = next_request;
                state.service_start_time = state.time;
                
                // Выбираем время обслуживания в зависимости от типа заявки
                if (next_request.type == REQUEST_TYPE_1)
                    service_time = generate_random_time(q1_service_min, q1_service_max);
                else
                    service_time = generate_random_time(q2_service_min, q2_service_max);
                    
                state.next_service_end = state.time + service_time;
            }
            else
            {
                state.device_busy = 0;
                state.current_request.type = 0;
                state.next_service_end = 0.0;
            }
        }
    }
    
    // Финальная статистика
    stats.total_modeling_time = state.time;
    if (printing)
    {
        printf("\n=== МОДЕЛИРОВАНИЕ ЗАВЕРШЕНО ===\n");
        print_statistics(&stats, 1);
    }
    
    return ERROR_OK;
}

// Функция вывода статистики
void print_statistics(const statistics_t *stats, int is_final)
{
    if (is_final)
    {
        printf("\n=== ФИНАЛЬНАЯ СТАТИСТИКА ===\n");
        printf("Общее время моделирования: %.2f е.в.\n", stats->total_modeling_time);
        printf("Время простоя аппарата: %.2f е.в.\n", stats->device_idle_time);
        printf("Коэффициент простоя: %.2f%%\n", 
               (stats->device_idle_time / stats->total_modeling_time) * 100);
        printf("Количество вошедших заявок 1 типа: %d\n", stats->total_requests_type1);
        printf("Количество вышедших заявок 1 типа: %d\n", stats->served_requests_type1);
        printf("Количество обращений заявки 2 типа: %d\n", stats->total_requests_type2); // <-- Это теперь отражает количество раз, когда заявка 2-го типа вошла в ОА
        printf("Максимальная длина очереди: %d\n", stats->max_queue_length);
        printf("Средняя длина очереди: %.2f\n", 
               stats->total_queue_length / stats->measurements_count);
        printf("Среднее время пребывания в очереди: %.2f е.в.\n", 
               stats->total_queue_time / stats->served_requests_type1);
    }
    else
    {
        printf("Промежуточная статистика:\n");
        printf("Обслужено заявок 1 типа: %d\n", stats->served_requests_type1);
        printf("Текущая длина очереди: измеряется в основном цикле\n");
    }
}

// Функция вывода промежуточной статистики
void print_intermediate_stats(const statistics_t *stats, int queue_size, int step)
{
    printf("\n--- Статистика после %d заявок 1 типа ---\n", step);
    printf("Текущая длина очереди: %d\n", queue_size);
    printf("Средняя длина очереди: %.2f\n", stats->total_queue_length / stats->measurements_count);
    printf("Вошедших заявок 1 типа: %d\n", stats->total_requests_type1);
    printf("Вышедших заявок 1 типа: %d\n", stats->served_requests_type1);
    if (stats->served_requests_type1 > 0)
        printf("Среднее время пребывания в очереди: %.2f\n", 
               stats->total_queue_time / stats->served_requests_type1);
    else
        printf("Среднее время пребывания в очереди: 0.00\n");
    printf("----------------------------------------\n\n");
}