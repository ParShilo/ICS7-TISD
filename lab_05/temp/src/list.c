#include "list.h"
#include "defines.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Добавление в память для отслеживания
void add_mem(void *addr, mem_t **mem)
{
    mem_t *new_mem = malloc(sizeof(mem_t));
    new_mem->address = addr;
    new_mem->busy = 1;
    new_mem->next = *mem;
    *mem = new_mem;
}

// Удаление из памяти
void delete_mem(void *addr, mem_t *mem)
{
    mem_t *current = mem;
    while (current != NULL)
    {
        if (current->address == addr)
        {
            current->busy = 0;
            break;
        }
        current = current->next;
    }
}

unsigned long simulate_list(mem_t **mem, int *mem_used, bool need_input)
{
    (void)need_input;
    
    queue_t queue = {0};
    oa_t oa = {0};
    
    double next_arrival_1 = get_time(COMING_START_1, COMING_END_1);
    int type2_in_oa = 1;
    
    unsigned long start_time = tick();
    
    printf("=== МОДЕЛИРОВАНИЕ ОЧЕРЕДИ (СПИСОК) ===\n");
    
    while (oa.processed_count_1 < TOTAL_NEED)
    {
        // Обработка заявки 2-го типа
        if (type2_in_oa && oa.time >= next_arrival_1)
        {
            double process_time = get_time(PROCESSING_START_2, PROCESSING_END_2);
            oa.time += process_time;
            oa.triggering++;
            oa.type2_requests++;
            
            node_t *new_node = malloc(sizeof(node_t));
            new_node->type = 2;
            new_node->arrival_time = oa.time;
            new_node->next = NULL;
            
            add_mem(new_node, mem);
            
            if (queue.list_len >= 4)
            {
                // Вставляем на 4-ю позицию
                node_t *current = queue.list_pout;
                for (int i = 0; i < 2; i++)
                {
                    current = current->next;
                }
                new_node->next = current->next;
                current->next = new_node;
            }
            else
            {
                // Добавляем в конец
                if (queue.list_pin != NULL)
                {
                    queue.list_pin->next = new_node;
                }
                queue.list_pin = new_node;
                if (queue.list_pout == NULL)
                {
                    queue.list_pout = new_node;
                }
            }
            queue.list_len++;
            type2_in_oa = 0;
        }
        
        // Добавление заявки 1-го типа
        if (next_arrival_1 <= oa.time)
        {
            node_t *new_node = malloc(sizeof(node_t));
            new_node->type = 1;
            new_node->arrival_time = next_arrival_1;
            new_node->next = NULL;
            
            add_mem(new_node, mem);
            
            if (queue.list_pin != NULL)
            {
                queue.list_pin->next = new_node;
            }
            queue.list_pin = new_node;
            if (queue.list_pout == NULL)
            {
                queue.list_pout = new_node;
            }
            
            queue.list_len++;
            queue.in_num++;
            next_arrival_1 += get_time(COMING_START_1, COMING_END_1);
        }
        
        // Обработка заявки из головы
        if (queue.list_len > 0 && !type2_in_oa && queue.list_pout->arrival_time <= oa.time)
        {
            queue.state += queue.list_len;
            if (queue.list_len > queue.max_len) 
                queue.max_len = queue.list_len;
            
            if (queue.list_pout->type == 1)
            {
                double process_time = get_time(PROCESSING_START_1, PROCESSING_END_1);
                oa.time += process_time;
                oa.triggering++;
                queue.total_stay_time += oa.time - queue.list_pout->arrival_time;
                oa.processed_count_1++;
                
                node_t *to_remove = queue.list_pout;
                queue.list_pout = queue.list_pout->next;
                if (queue.list_pout == NULL)
                {
                    queue.list_pin = NULL;
                }
                queue.list_len--;
                
                delete_mem(to_remove, *mem);
                free(to_remove);
                
                if (oa.processed_count_1 % INTER_NEED == 0)
                {
                    double avg_len = (double)queue.state / oa.triggering;
                    double avg_stay = queue.total_stay_time / oa.processed_count_1;
                    print_intermediate_stats(oa.processed_count_1, queue.list_len, 
                                           avg_len, queue.in_num, avg_stay);
                }
            }
            else
            {
                type2_in_oa = 1;
                node_t *to_remove = queue.list_pout;
                queue.list_pout = queue.list_pout->next;
                if (queue.list_pout == NULL)
                {
                    queue.list_pin = NULL;
                }
                queue.list_len--;
                
                delete_mem(to_remove, *mem);
                free(to_remove);
            }
        }
        else if (queue.list_len == 0 && !type2_in_oa)
        {
            oa.downtime += next_arrival_1 - oa.time;
            oa.time = next_arrival_1;
        }
    }
    
    unsigned long end_time = tick() - start_time;
    
    // Расчет памяти
    *mem_used = queue.max_len * sizeof(node_t);
    double expected_time = ((COMING_END_1 + COMING_START_1) / 2.0) * queue.in_num;
    double error = fabs((oa.time - expected_time) / expected_time * 100);
    
    print_final_stats(oa.time, oa.downtime, queue.in_num, 
                     oa.processed_count_1, oa.type2_requests, error);
    
    printf("Занимаемая память: %d байт\n", *mem_used);
    printf("Время выполнения: %lu тиков\n", end_time);
    
    return end_time;
}

void show_mem(mem_t **mem)
{
    printf("\n=== АДРЕСА ПАМЯТИ (СПИСОК) ===\n");
    mem_t *current = *mem;
    int count = 0, reused = 0;
    
    while (current != NULL && count < 50)
    {
        printf("Адрес: %p, Статус: %s\n", 
               current->address, 
               current->busy ? "занят" : "свободен");
        if (!current->busy) reused++;
        current = current->next;
        count++;
    }
    
    printf("\nПовторно использовано адресов: %d\n", reused);
    printf("Фрагментация памяти: %s\n", reused > 0 ? "есть" : "нет");
}