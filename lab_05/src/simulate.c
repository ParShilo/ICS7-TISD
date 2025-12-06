#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

// Основная функция моделирования для массива
int simulate_array_queue(int printing, statistics_t *stats_out)
{
    array_queue queue;
    system_state_t state;
    statistics_t stats;
    
    double next_event_time, service_time;
    int event_type, insert_position, rc = ERROR_OK;
    request_t next_request, type2_request;

    // Инициализация
    init_array_queue(&queue);
    
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
        printf("\n\n=== МОДЕЛИРОВАНИЕ СИСТЕМЫ МАССОВОГО ОБСЛУЖИВАНИЯ (МАССИВ) ===\n");
        printf("Параметры системы:\n");
        printf("  Заявки 1 типа: приход [%.1f, %.1f], обслуживание [%.1f, %.1f]\n", q1_arrival_min, q1_arrival_max, q1_service_min, q1_service_max);
        printf("  Заявки 2 типа: обслуживание [%.1f, %.1f]\n", q2_service_min, q2_service_max);
        printf("Начало моделирования...\n\n");
    }
    
    // Вхождение заявки 2-го типа в ОА
    state.device_busy = 1;
    state.current_request.type = REQUEST_TYPE_2;
    state.current_request.arrival_time = 0.0;
    state.service_start_time = 0.0;
    state.next_service_end = generate_random_time(q2_service_min, q2_service_max);
    stats.total_requests_type2++;

    // Основной цикл моделирования
    while (stats.served_requests_type1 < 1000)
    {
        // Определение следующего события
        next_event_time = state.next_arrival_type1;
        event_type = 1; // 1 - приход заявки 1 типа, 2 - окончание обслуживания
        
        if (state.device_busy && state.next_service_end < next_event_time)
        {
            next_event_time = state.next_service_end;
            event_type = 2;
        }
        
        // Обновление статистики длины очереди
        stats.total_queue_length += queue.size;
        stats.measurements_count++;
        if (queue.size > stats.max_queue_length)
            stats.max_queue_length = queue.size;
        
        // Обновление времени простоя аппарата
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

            // Создание заявки 1 типа
            request_t new_request = {REQUEST_TYPE_1, state.time};
            
            // Добавление заявки в очередь
            if (enqueue_array(&queue, new_request, 0) == ERROR_OK)
                stats.total_requests_type1++;
            
            // Планирование следующего прихода
            state.next_arrival_type1 = state.time + generate_random_time(q1_arrival_min, q1_arrival_max);
        }
        else // Окончание обслуживания
        {
            if (state.current_request.type == REQUEST_TYPE_1)
            {
                stats.served_requests_type1++;
                stats.total_queue_time += (state.service_start_time - state.current_request.arrival_time);
                
                if (stats.served_requests_type1 % 100 == 0 && printing)
                    print_intermediate_stats(&stats, queue.size, stats.served_requests_type1);
            }
            else // REQUEST_TYPE_2
            {
                // Возвращение заявки 2-го типа в очередь
                type2_request = state.current_request;
                type2_request.arrival_time = state.time;
                stats.total_requests_type2++;
                
                insert_position = (queue.size >= 3) ? 3 : queue.size;
                rc = insert_array_element_at_position(&queue, type2_request, insert_position);
                if (rc == ERROR_OVEFLOW && printing)
                    printf("Время %.2f: ОЧЕРЕДЬ ПЕРЕПОЛНЕНА!\n", state.time);
                if (rc != ERROR_OK)
                    return rc;
            }
            
            if (!is_array_queue_empty(&queue))
            {
                next_request = dequeue_array(&queue, 0);
                
                state.device_busy = 1;
                state.current_request = next_request;
                state.service_start_time = state.time;
                
                // Выбор времени обслуживания в зависимости от типа заявки
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
        print_statistics(&stats);

    // Выход статистики
    if (stats_out != NULL)
        *stats_out = stats;
    
    return ERROR_OK;
}

// Моделирование для списка
int simulate_list_queue(int printing, statistics_t *stats_out)
{
    list_queue queue;
    system_state_t state;
    statistics_t stats;
    
    double next_event_time, service_time;
    int event_type, rc = ERROR_OK;
    request_t next_request, type2_request;

    init_list_queue(&queue);
    srand(time(NULL));
    
    // Начальное состояние
    state.time = 0.0;
    state.next_arrival_type1 = generate_random_time(q1_arrival_min, q1_arrival_max);
    state.next_service_end = 0.0;
    state.device_busy = 0;
    state.current_request.type = 0;
    state.current_request.arrival_time = 0.0;
    state.service_start_time = 0.0;
    
    // Статистика
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
        printf("\n\n=== МОДЕЛИРОВАНИЕ СИСТЕМЫ МАССОВОГО ОБСЛУЖИВАНИЯ (СПИСОК) ===\n");
        printf("Параметры системы:\n");
        printf("  Заявки 1 типа: приход [%.1f, %.1f], обслуживание [%.1f, %.1f]\n", q1_arrival_min, q1_arrival_max, q1_service_min, q1_service_max);
        printf("  Заявки 2 типа: обслуживание [%.1f, %.1f]\n", q2_service_min, q2_service_max);
        printf("Начало моделирования...\n\n");
    }
    
    // Вхождение заявки 2-го типа в ОА
    state.device_busy = 1;
    state.current_request.type = REQUEST_TYPE_2;
    state.current_request.arrival_time = 0.0;
    state.service_start_time = 0.0;
    state.next_service_end = generate_random_time(q2_service_min, q2_service_max);
    stats.total_requests_type2++;

    while (stats.served_requests_type1 < 1000)
    {
        next_event_time = state.next_arrival_type1;
        event_type = 1;
        
        if (state.device_busy && state.next_service_end < next_event_time)
        {
            next_event_time = state.next_service_end;
            event_type = 2;
        }
        
        stats.total_queue_length += queue.size;
        stats.measurements_count++;            
        if (queue.size > stats.max_queue_length)
            stats.max_queue_length = queue.size;
        
        if (!state.device_busy)
            stats.device_idle_time += (next_event_time - state.time);
        
        state.time = next_event_time;
        
        if (event_type == 1)
        {
            if (queue.size >= MAX_QUEUE_SIZE)
            {
                if (printing)
                    printf("Время %.2f: ОЧЕРЕДЬ ПЕРЕПОЛНЕНА!\n", state.time);
                return ERROR_OVEFLOW;
            }

            request_t new_request = {REQUEST_TYPE_1, state.time};
            rc = enqueue_list(&queue, new_request, 0);
            if (rc == ERROR_MEM)
                return ERROR_MEM;
            stats.total_requests_type1++;
            
            state.next_arrival_type1 = state.time + generate_random_time(q1_arrival_min, q1_arrival_max);
        }
        else
        {
            if (state.current_request.type == REQUEST_TYPE_1)
            {
                stats.served_requests_type1++;
                stats.total_queue_time += (state.service_start_time - state.current_request.arrival_time);
                
                if (stats.served_requests_type1 % 100 == 0 && printing)
                    print_intermediate_stats(&stats, queue.size, stats.served_requests_type1);
            }
            else
            {
                type2_request = state.current_request;
                type2_request.arrival_time = state.time;
                stats.total_requests_type2++;
                
                rc = insert_list_element_at_position(&queue, type2_request, 3);
                if (rc == ERROR_OVEFLOW && printing)
                    printf("Время %.2f: ОЧЕРЕДЬ ПЕРЕПОЛНЕНА!\n", state.time);
                if (rc != ERROR_OK)
                    return rc;
            }
            
            if (!is_list_queue_empty(&queue))
            {
                next_request = dequeue_list(&queue, 0);
                state.device_busy = 1;
                state.current_request = next_request;
                state.service_start_time = state.time;
                
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
    
    stats.total_modeling_time = state.time;
    if (printing)
        print_statistics(&stats);
    
    // Выход статистики
    if (stats_out != NULL)
        *stats_out = stats;

    return ERROR_OK;
}

// Функция для замера времени
uint64_t tick(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

// Функция сравнения производительности
int compare_queues(void)
{
    int runs = 1000, mem_array, mem_list, rc = ERROR_OK, max_queue_len_list = 0;
    uint64_t time_array = 0, time_list = 0, start, end;

    printf("\n=== СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ===\n");
    printf("Количество запусков: %d\n", runs);

    // Оценка памяти
    mem_array = MAX_QUEUE_SIZE * sizeof(request_t);

    // Замер времени для массива
    for (int i = 0; i < runs; i++)
    {
        srand(runs + i);
        start = tick();
        rc = simulate_array_queue(0, NULL);
        end = tick();
        if (rc != ERROR_OK)
            return rc;
        time_array += (end - start);
    }

    // Замер времени для списка
    for (int i = 0; i < runs; i++)
    {
        srand(runs + i);
        statistics_t stats_list = {0};
        start = tick();
        rc = simulate_list_queue(0, &stats_list);
        end = tick();
        if (rc != ERROR_OK)
            return rc;
        time_list += (end - start);
        max_queue_len_list += stats_list.max_queue_length;
    }

    // Вывод результатов
    double avg_time_array = time_array / (double)runs / 1e6;
    double avg_time_list = time_list  / (double)runs / 1e6;
    mem_list =  (max_queue_len_list / runs) * sizeof(node_t);

    printf("+-----------------------------------------------------------+\n");
    printf("|                        РЕЗУЛЬТАТЫ                         |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|                        | Массив-очередь  | Список-очередь |\n");
    printf("+------------------------+-----------------+----------------+\n");
    printf("| Время (мс)             | %15.2f | %14.2f |\n", avg_time_array, avg_time_list);
    printf("| Память (байт)          |  %14d | %14d |\n", mem_array, mem_list);
    printf("+------------------------+-----------------+----------------+\n");

    return ERROR_OK;
}