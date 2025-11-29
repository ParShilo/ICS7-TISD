#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "defines.h"
#include "array_queue.h"
#include "list_queue.h"
#include "simulation.h"

void init_oa(oa_t *oa) {
    oa->current_time = 0;
    oa->downtime = 0;
    oa->processed_count_1 = 0;
    oa->processed_count_2 = 0;
    oa->triggering = 0;
}

void simulate_array_queue(int *mem_used, bool show_stats)
{
    array_queue_t queue;
    oa_t oa;
    
    if (!init_array_queue(&queue, MAX_QUEUE_SIZE)) {
        printf("Ошибка инициализации очереди-массива!\n");
        return;
    }
    
    init_oa(&oa);
    
    double next_arrival_1 = get_time(COMING_START_1, COMING_END_1);
    double next_processing_2 = 0;
    int type2_processed = 0;
    
    int stats_counter = 0;
    int total_queue_length = 0;
    int measurements_count = 0;
    
    uint64_t start_time = tick();
    
    while (oa.processed_count_1 < TOTAL_NEED) {
        int action_taken = 0;
        
        if (next_arrival_1 <= oa.current_time && oa.processed_count_1 + queue.in_num < TOTAL_NEED + queue.len) {
            request_t new_req = {1, next_arrival_1, 0};
            if (!add_to_array_queue(&queue, new_req)) {
                printf("Переполнение очереди-массива! Прекращение обработки.\n");
                break;
            }
            next_arrival_1 = oa.current_time + get_time(COMING_START_1, COMING_END_1);
            action_taken = 1;
        }
        
        if (next_processing_2 <= oa.current_time) {
            //request_t processed_req = {2, oa.current_time, 0};
            type2_processed++;
            oa.triggering++;
            
            double process_time = get_time(PROCESSING_START_2, PROCESSING_END_2);
            oa.current_time += process_time;
            next_processing_2 = oa.current_time;
            action_taken = 1;
        }
        
        if (queue.len > 0 && !action_taken) {
            request_t req;
            if (remove_from_array_queue(&queue, &req)) {
                oa.triggering++;
                
                if (req.type == 1) {
                    double process_time = get_time(PROCESSING_START_1, PROCESSING_END_1);
                    queue.total_stay_time += oa.current_time - req.arrival_time;
                    oa.current_time += process_time;
                    oa.processed_count_1++;
                    
                    stats_counter++;
                    if (stats_counter >= INTER_NEED && show_stats) {
                        double avg_queue_length = (double)total_queue_length / measurements_count;
                        printf("\nПосле обработки %d заявок 1 типа:\n", oa.processed_count_1);
                        printf("Текущая длина очереди: %d\n", queue.len);
                        printf("Средняя длина очереди: %.2f\n", avg_queue_length);
                        printf("Вошедших заявок: %d, Вышедших заявок: %d\n", 
                               queue.in_num, oa.processed_count_1);
                        printf("Среднее время пребывания: %.2f\n", 
                               queue.total_stay_time / oa.processed_count_1);
                        
                        stats_counter = 0;
                        total_queue_length = 0;
                        measurements_count = 0;
                    }
                }
            }
        }
        
        if (queue.len > 0 && oa.current_time < next_arrival_1 && oa.current_time < next_processing_2) {
            oa.current_time = next_arrival_1 < next_processing_2 ? next_arrival_1 : next_processing_2;
        }
        
        if (queue.len == 0 && oa.current_time < next_arrival_1) {
            oa.downtime += next_arrival_1 - oa.current_time;
            oa.current_time = next_arrival_1;
        }
        
        total_queue_length += queue.len;
        measurements_count++;
    }
    
    uint64_t end_time = tick();
    uint64_t exec_time = end_time - start_time;
    
    if (show_stats) {
        printf("\n=== ОКОНЧАНИЕ МОДЕЛИРОВАНИЯ (МАССИВ) ===\n");
        printf("Общее время моделирования: %.2f е.в.\n", oa.current_time);
        printf("Время простоя ОА: %.2f е.в.\n", oa.downtime);
        printf("Заявок 1 типа: вошло %d, вышло %d\n", queue.in_num, oa.processed_count_1);
        printf("Обращений заявки 2 типа: %d\n", type2_processed);
        printf("Время выполнения программы: %lu тиков\n", exec_time);
    }
    
    *mem_used = queue.max_len * sizeof(request_t);
    free_array_queue(&queue);
}

void simulate_list_queue(int *mem_used, mem_t **mem, bool show_stats) {
    list_queue_t queue;
    oa_t oa;
    
    init_list_queue(&queue);
    init_oa(&oa);
    
    double next_arrival_1 = get_time(COMING_START_1, COMING_END_1);
    double next_processing_2 = 0;
    int type2_processed = 0;
    
    int stats_counter = 0;
    int total_queue_length = 0;
    int measurements_count = 0;
    
    uint64_t start_time = tick();
    
    while (oa.processed_count_1 < TOTAL_NEED) {
        int action_taken = 0;
        
        if (next_arrival_1 <= oa.current_time && oa.processed_count_1 + queue.in_num < TOTAL_NEED + queue.len) {
            request_t new_req = {1, next_arrival_1, 0};
            if (!add_to_list_queue(&queue, new_req, mem)) {
                printf("Ошибка добавления в очередь-список! Прекращение обработки.\n");
                break;
            }
            next_arrival_1 = oa.current_time + get_time(COMING_START_1, COMING_END_1);
            action_taken = 1;
        }
        
        if (next_processing_2 <= oa.current_time) {
            //request_t processed_req = {2, oa.current_time, 0};
            type2_processed++;
            oa.triggering++;
            
            double process_time = get_time(PROCESSING_START_2, PROCESSING_END_2);
            oa.current_time += process_time;
            next_processing_2 = oa.current_time;
            action_taken = 1;
        }
        
        if (queue.len > 0 && !action_taken) {
            request_t req;
            if (remove_from_list_queue(&queue, &req, *mem)) {
                oa.triggering++;
                
                if (req.type == 1) {
                    double process_time = get_time(PROCESSING_START_1, PROCESSING_END_1);
                    queue.total_stay_time += oa.current_time - req.arrival_time;
                    oa.current_time += process_time;
                    oa.processed_count_1++;
                    
                    stats_counter++;
                    if (stats_counter >= INTER_NEED && show_stats) {
                        double avg_queue_length = (double)total_queue_length / measurements_count;
                        printf("\nПосле обработки %d заявок 1 типа:\n", oa.processed_count_1);
                        printf("Текущая длина очереди: %d\n", queue.len);
                        printf("Средняя длина очереди: %.2f\n", avg_queue_length);
                        printf("Вошедших заявок: %d, Вышедших заявок: %d\n", 
                               queue.in_num, oa.processed_count_1);
                        printf("Среднее время пребывания: %.2f\n", 
                               queue.total_stay_time / oa.processed_count_1);
                        
                        stats_counter = 0;
                        total_queue_length = 0;
                        measurements_count = 0;
                    }
                }
            }
        }
        
        if (queue.len > 0 && oa.current_time < next_arrival_1 && oa.current_time < next_processing_2) {
            oa.current_time = next_arrival_1 < next_processing_2 ? next_arrival_1 : next_processing_2;
        }
        
        if (queue.len == 0 && oa.current_time < next_arrival_1) {
            oa.downtime += next_arrival_1 - oa.current_time;
            oa.current_time = next_arrival_1;
        }
        
        total_queue_length += queue.len;
        measurements_count++;
    }
    
    uint64_t end_time = tick();
    uint64_t exec_time = end_time - start_time;
    
    if (show_stats) {
        printf("\n=== ОКОНЧАНИЕ МОДЕЛИРОВАНИЯ (СПИСОК) ===\n");
        printf("Общее время моделирования: %.2f е.в.\n", oa.current_time);
        printf("Время простоя ОА: %.2f е.в.\n", oa.downtime);
        printf("Заявок 1 типа: вошло %d, вышло %d\n", queue.in_num, oa.processed_count_1);
        printf("Обращений заявки 2 типа: %d\n", type2_processed);
        printf("Время выполнения программы: %lu тиков\n", exec_time);
    }
    
    *mem_used = queue.max_len * sizeof(queue_node_t);
    free_list_queue(&queue);
}

void show_memory_addresses(mem_t *mem) {
    printf("\nАдреса памяти, используемые очередью-списком:\n");
    int count = 0;
    mem_t *current = mem;
    
    while (current != NULL && count < 50) {
        printf("Адрес: %p, Статус: %s\n", 
               current->address, 
               current->busy ? "занят" : "свободен");
        current = current->next;
        count++;
    }
    
    if (count == 50) {
        printf("... (показаны первые 50 адресов)\n");
    }
}

void compare_queues() {
    int mem_array = 0, mem_list = 0;
    mem_t *mem = NULL;
    
    printf("\n=== СРАВНИТЕЛЬНЫЙ АНАЛИЗ ===\n");
    
    uint64_t time_array = tick();
    simulate_array_queue(&mem_array, false);
    time_array = tick() - time_array;
    
    uint64_t time_list = tick();
    simulate_list_queue(&mem_list, &mem, false);
    time_list = tick() - time_list;
    
    printf("\n+------------------------------------------+\n");
    printf("| %-20s | %-15s |\n", "Параметр", "Значение");
    printf("+------------------------------------------+\n");
    printf("| %-20s | %-15lu |\n", "Время массива (тики)", time_array);
    printf("| %-20s | %-15lu |\n", "Время списка (тики)", time_list);
    printf("| %-20s | %-15d |\n", "Память массива (байт)", mem_array);
    printf("| %-20s | %-15d |\n", "Память списка (байт)", mem_list);
    printf("+------------------------------------------+\n");
    
    mem_t *current = mem;
    while (current != NULL) {
        mem_t *temp = current;
        current = current->next;
        free(temp);
    }
}