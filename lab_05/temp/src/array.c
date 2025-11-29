#include "array.h"
#include "defines.h"
#include "io.h"
#include <stdio.h>
#include <math.h>

typedef struct {
    int type;
    double arrival_time;
} array_element_t;

unsigned long simulate_array(int *mem_used, bool need_input)
{
    (void)need_input;
    
    // Статический массив
    array_element_t array[MAX_QUEUE_SIZE];
    
    // Инициализация очереди
    queue_t queue = {0};
    queue.array_pin = 0;
    queue.array_pout = 0;
    queue.array_len = 0;
    
    // Инициализация ОА
    oa_t oa = {0};
    
    double next_arrival_1 = get_time(COMING_START_1, COMING_END_1);
    int type2_in_oa = 1; // Заявка 2-го типа начинает в ОА
    
    unsigned long start_time = tick();
    
    printf("=== МОДЕЛИРОВАНИЕ ОЧЕРЕДИ (СТАТИЧЕСКИЙ МАССИВ) ===\n");
    
    while (oa.processed_count_1 < TOTAL_NEED)
    {
        // Обработка заявки 2-го типа
        if (type2_in_oa && oa.time >= next_arrival_1)
        {
            double process_time = get_time(PROCESSING_START_2, PROCESSING_END_2);
            oa.time += process_time;
            oa.triggering++;
            oa.type2_requests++;
            
            // Возвращаем заявку 2-го типа на 4-ю позицию или в конец
            if (queue.array_len >= 4)
            {
                // Сохраняем 4-й элемент
                int fourth_pos = (queue.array_pout + 3) % MAX_QUEUE_SIZE;
                array_element_t temp = array[fourth_pos];
                
                // Сдвигаем элементы
                for (int i = fourth_pos; i != queue.array_pin; i = (i + 1) % MAX_QUEUE_SIZE)
                {
                    int next = (i + 1) % MAX_QUEUE_SIZE;
                    array[i] = array[next];
                }
                
                // Возвращаем сохраненный элемент в конец как заявку 2-го типа
                queue.array_pin = (queue.array_pin - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
                array[queue.array_pin] = temp;
                array[queue.array_pin].type = 2;
                array[queue.array_pin].arrival_time = oa.time;
            }
            else
            {
                // Добавляем в конец
                if (queue.array_len < MAX_QUEUE_SIZE)
                {
                    array[queue.array_pin].type = 2;
                    array[queue.array_pin].arrival_time = oa.time;
                    queue.array_pin = (queue.array_pin + 1) % MAX_QUEUE_SIZE;
                    queue.array_len++;
                }
            }
            type2_in_oa = 0;
        }
        
        // Добавление заявки 1-го типа
        if (next_arrival_1 <= oa.time && queue.array_len < MAX_QUEUE_SIZE)
        {
            array[queue.array_pin].type = 1;
            array[queue.array_pin].arrival_time = next_arrival_1;
            queue.array_pin = (queue.array_pin + 1) % MAX_QUEUE_SIZE;
            queue.array_len++;
            queue.in_num++;
            next_arrival_1 += get_time(COMING_START_1, COMING_END_1);
        }
        
        // Обработка заявки из головы очереди
        if (queue.array_len > 0 && !type2_in_oa && array[queue.array_pout].arrival_time <= oa.time)
        {
            queue.state += queue.array_len;
            if (queue.array_len > queue.max_len) 
                queue.max_len = queue.array_len;
            
            if (array[queue.array_pout].type == 1)
            {
                // Обработка заявки 1-го типа
                double process_time = get_time(PROCESSING_START_1, PROCESSING_END_1);
                oa.time += process_time;
                oa.triggering++;
                queue.total_stay_time += oa.time - array[queue.array_pout].arrival_time;
                oa.processed_count_1++;
                queue.array_pout = (queue.array_pout + 1) % MAX_QUEUE_SIZE;
                queue.array_len--;
                
                // Промежуточная статистика
                if (oa.processed_count_1 % INTER_NEED == 0)
                {
                    double avg_len = (double)queue.state / oa.triggering;
                    double avg_stay = queue.total_stay_time / oa.processed_count_1;
                    print_intermediate_stats(oa.processed_count_1, queue.array_len, 
                                           avg_len, queue.in_num, avg_stay);
                }
            }
            else
            {
                // Заявка 2-го типа уходит в ОА
                type2_in_oa = 1;
                queue.array_pout = (queue.array_pout + 1) % MAX_QUEUE_SIZE;
                queue.array_len--;
            }
        }
        else if (queue.array_len == 0 && !type2_in_oa)
        {
            // Простой ОА
            oa.downtime += next_arrival_1 - oa.time;
            oa.time = next_arrival_1;
        }
    }
    
    unsigned long end_time = tick() - start_time;
    
    // Расчет памяти и погрешности
    *mem_used = MAX_QUEUE_SIZE * sizeof(array_element_t);
    double expected_time = ((COMING_END_1 + COMING_START_1) / 2.0) * queue.in_num;
    double error = fabs((oa.time - expected_time) / expected_time * 100);
    
    print_final_stats(oa.time, oa.downtime, queue.in_num, 
                     oa.processed_count_1, oa.type2_requests, error);
    
    printf("Занимаемая память: %d байт\n", *mem_used);
    printf("Время выполнения: %lu тиков\n", end_time);
    
    return end_time;
}