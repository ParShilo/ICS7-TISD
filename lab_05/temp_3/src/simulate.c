#include "simulate.h"

extern float q1_arrival_min, q1_arrival_max;
extern float q1_service_min, q1_service_max;
extern float q2_service_min, q2_service_max;

void print_intermediate_results(SimulationLog *log, int batch_count)
{
    printf("\n=== После обработки %d заявок 1-го типа ===\n", batch_count * 100);
    printf("Текущая длина очереди: %d\n", log->current_length);
    printf("Средняя длина очереди: %.2f\n", 
           log->tasks_out > 0 ? (float)log->total_length / log->tasks_out : 0.0f);
    printf("Вошедших заявок: %d\n", log->tasks_in);
    printf("Вышедших заявок: %d\n", log->tasks_out);
    printf("Среднее время пребывания в очереди: %.2f\n",
           log->tasks_out > 0 ? log->total_wait_time / log->tasks_out : 0.0f);
    printf("==========================================\n\n");
}

void print_final_results(float total_time, float idle_time, SimulationLog *log1, SimulationLog *log2)
{
    printf("\n=== ФИНАЛЬНЫЕ РЕЗУЛЬТАТЫ ===\n");
    printf("Общее время моделирования: %.2f е.в.\n", total_time);
    printf("Время простоя ОА: %.2f е.в.\n", idle_time);
    printf("Заявки 1-го типа: вошедших=%d, вышедших=%d\n", 
           log1->tasks_in, log1->tasks_out);
    printf("Заявки 2-го типа: количество обращений=%d\n", 
           log2->tasks_out);
    
    float expected_time = (q1_arrival_min + q1_arrival_max) / 2.0f * 1000.0f;
    float error = fabsf(total_time - expected_time) / expected_time * 100.0f;
    printf("Ожидаемое время: %.2f е.в.\n", expected_time);
    printf("Погрешность: %.2f%%\n", error);
}

void simulate_arr(int key)
{
    float current_time = 0.0f;
    float next_arrival_time = random_float(q1_arrival_min, q1_arrival_max);
    float next_service_time = 0.0f;
    float idle_time = 0.0f;
    
    arr_queue_t *queue = create_arr();
    SimulationLog log1 = {0};
    SimulationLog log2 = {0};
    
    // Добавляем заявку 2-го типа в начало
    task_t *type2_task = create_task(TYPE2);
    push_arr(queue, type2_task);
    log2.tasks_in = 1;
    
    int batch_count = 0;
    
    while (log1.tasks_out < 1000)
    {
        // Обработка прихода заявки 1-го типа
        if (current_time >= next_arrival_time)
        {
            task_t *new_task = create_task(TYPE1);
            if (push_arr(queue, new_task) == 0)
            {
                log1.tasks_in++;
                log1.current_length = queue->amount;
            }
            else
            {
                log1.failed_tasks++;
                delete_task(new_task);
            }
            next_arrival_time = current_time + random_float(q1_arrival_min, q1_arrival_max);
        }
        
        // Обработка обслуживания в ОА
        if (current_time >= next_service_time && queue->amount > 0)
        {
            task_t *current_task = pop_arr(queue);
            
            if (current_task->type == TYPE1)
            {
                // Заявка 1-го типа покидает систему после обслуживания
                next_service_time = current_time + random_float(q1_service_min, q1_service_max);
                log1.tasks_out++;
                log1.total_length += queue->amount;
                log1.total_wait_time += current_time; // упрощенное время ожидания
                delete_task(current_task);
                
                // Вывод промежуточных результатов
                if (log1.tasks_out % 100 == 0)
                {
                    batch_count++;
                    if (key) print_intermediate_results(&log1, batch_count);
                }
            }
            else
            {
                // Заявка 2-го типа возвращается в очередь
                next_service_time = current_time + random_float(q2_service_min, q2_service_max);
                log2.tasks_out++;
                
                // Возврат на 4-ю позицию от головы или в конец
                if (queue->amount >= 3)
                {
                    // Вставка на 4-ю позицию
                    task_t **tmp = malloc((queue->amount + 1) * sizeof(task_t*));
                    for (int i = 0; i < 3 && i < queue->amount; i++)
                    {
                        tmp[i] = queue->data[i];
                    }
                    tmp[3] = current_task;
                    for (int i = 3; i < queue->amount; i++)
                    {
                        tmp[i + 1] = queue->data[i];
                    }
                    free(queue->data);
                    queue->data = tmp;
                    queue->amount++;
                    queue->Pin++;
                }
                else
                {
                    // Возврат в конец очереди
                    push_arr(queue, current_task);
                }
            }
            
            log1.current_length = queue->amount;
        }
        
        // Расчет времени до следующего события
        float time_until_arrival = next_arrival_time - current_time;
        float time_until_service = (queue->amount > 0) ? next_service_time - current_time : 1e9f;
        
        float time_step = min_float(time_until_arrival, time_until_service);
        
        if (time_step > 1e8f) // Если нет активных событий
        {
            idle_time += time_until_arrival;
            time_step = time_until_arrival;
        }
        
        current_time += time_step;
    }
    
    if (key) print_final_results(current_time, idle_time, &log1, &log2);
    delete_arr(queue);
}

void simulate_list(int key)
{
    float current_time = 0.0f;
    float next_arrival_time = random_float(q1_arrival_min, q1_arrival_max);
    float next_service_time = 0.0f;
    float idle_time = 0.0f;
    
    list_queue_t *queue = create_list();
    SimulationLog log1 = {0};
    SimulationLog log2 = {0};
    
    // Добавляем заявку 2-го типа в начало
    task_t *type2_task = create_task(TYPE2);
    push_list(queue, type2_task);
    log2.tasks_in = 1;
    
    int batch_count = 0;
    
    while (log1.tasks_out < 1000)
    {
        // Обработка прихода заявки 1-го типа
        if (current_time >= next_arrival_time)
        {
            task_t *new_task = create_task(TYPE1);
            if (push_list(queue, new_task) == 0)
            {
                log1.tasks_in++;
                log1.current_length = queue->amount;
                if (key) printf("Добавлена заявка 1-го типа. Адрес: %p\n", (void*)new_task);
            }
            else
            {
                log1.failed_tasks++;
                delete_task(new_task);
            }
            next_arrival_time = current_time + random_float(q1_arrival_min, q1_arrival_max);
        }
        
        // Обработка обслуживания в ОА
        if (current_time >= next_service_time && queue->amount > 0)
        {
            task_t *current_task = pop_list(queue);
            if (key) printf("Удалена заявка из очереди. Адрес: %p, Тип: %d\n", 
                           (void*)current_task, current_task->type);
            
            if (current_task->type == TYPE1)
            {
                next_service_time = current_time + random_float(q1_service_min, q1_service_max);
                log1.tasks_out++;
                log1.total_length += queue->amount;
                log1.total_wait_time += current_time;
                delete_task(current_task);
                
                if (log1.tasks_out % 100 == 0)
                {
                    batch_count++;
                    if (key) print_intermediate_results(&log1, batch_count);
                }
            }
            else
            {
                next_service_time = current_time + random_float(q2_service_min, q2_service_max);
                log2.tasks_out++;
                
                // Возврат на 4-ю позицию
                if (queue->amount >= 3)
                {
                    // Находим 3-й элемент
                    node_t *third = queue->Pin;
                    for (int i = 0; i < 2 && third != NULL; i++)
                    {
                        third = third->next;
                    }
                    
                    if (third != NULL)
                    {
                        node_t *new_node = create_node(current_task);
                        new_node->next = third->next;
                        third->next = new_node;
                        queue->amount++;
                    }
                    else
                    {
                        push_list(queue, current_task);
                    }
                }
                else
                {
                    push_list(queue, current_task);
                }
            }
            
            log1.current_length = queue->amount;
        }
        
        float time_until_arrival = next_arrival_time - current_time;
        float time_until_service = (queue->amount > 0) ? next_service_time - current_time : 1e9f;
        
        float time_step = min_float(time_until_arrival, time_until_service);
        
        if (time_step > 1e8f)
        {
            idle_time += time_until_arrival;
            time_step = time_until_arrival;
        }
        
        current_time += time_step;
    }
    
    if (key) print_final_results(current_time, idle_time, &log1, &log2);
    delete_list(queue);
}

float random_float(float min, float max)
{
    if (min >= max) return min;
    return (float)rand() / (float)RAND_MAX * (max - min) + min;
}

float min_float(float a, float b)
{
    return (a < b) ? a : b;
}