#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "../inc/err.h"
#include "stack.h"
#include "print.h"
#include "array_stack.h"
#include "list_stack.h"
#include "main.h"

int main(void)
{
    int choice;
    
    printf("Программа работы со стеком\n");
    
    do {
        print_main_menu();
        choice = get_integer_input("Введите номер", 0, 2);
        
        switch (choice)
        {
            case 1:
            {
                emulate_stack();
                break;
            }
            case 2:
            {
                compare_stacks();
                break;
            }
            case 0:
            {
                printf("Выход из программы.\n");
                break;
            }
            default:
                printf("Неверный выбор!\n");
        }
    } while (choice != 0);
    
    return 0;
}

void emulate_stack(void)
{
    int stack_type;
    
    printf("Эмуляция стека\n");
    
    do {
        print_stack_type_menu();
        stack_type = get_integer_input("Введите номер", 0, 2);
        
        if (stack_type == 0) return;
        
        if (stack_type == 1)
        {
            ArrayStack stack;
            array_init(&stack);
            array_stack_operations(&stack);
        }
        else if (stack_type == 2) 
        {
            ListStack stack;
            list_init(&stack);
            list_stack_operations(&stack);
            list_free(&stack);
        }
    } while (stack_type != 0);
}

void compare_stacks(void)
{
    printf("\n");
    printf("    +-----------------------------------------------------------------------------------------+\n");
    printf("    |                               Сравнение реализации стеков                               |\n");
    printf("    +-----------------------------------------------------------------------------------------+\n");
    
    int sizes[] = {10, 50, 100, 300, 500, 1000, 2000, 5000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const int NUM_RUNS = 10;
    
    printf("    | Размер | Массив(сек) | Список(сек) | Время(м/с) | Память(массив) | Память(список) | Память(м/с) |\n");
    printf("    +--------+-------------+-------------+------------+----------------+----------------+-------------+\n");
    
    ArrayStack stack1, stack2, temp, result;
    ListStack list_stack1, list_stack2, list_temp, list_result;
    int n, value, temp_val;
    double total_array_time, total_list_time, avg_array_time, avg_list_time, time_ratio, memory_ratio;
    size_t array_memory, list_memory;
    clock_t start, end;
    
    for (int i = 0; i < num_sizes; i++)
    {
        n = sizes[i];
        total_array_time = 0.0;
        total_list_time = 0.0;
        
        for (int run = 0; run < NUM_RUNS; run++)
        {
            // Инициализация стеков
            array_init(&stack1);
            array_init(&stack2);
            array_init(&temp);
            array_init(&result);
            
            list_init(&list_stack1);
            list_init(&list_stack2);
            list_init(&list_temp);
            list_init(&list_result);
            
            // Заполнение случайными числами
            srand(time(NULL) + run);
            
            for (int j = 0; j < n/2; j++)
            {
                value = rand() % 1000;
                array_push(&stack1, value);
                list_push(&list_stack1, value);
            }
            for (int j = 0; j < n/2; j++)
            {
                value = rand() % 1000;
                array_push(&stack2, value);
                list_push(&list_stack2, value);
            }

            //printf("Случайный и случайный: \n");
            //list_display(&list_stack1);
            //array_display(&stack1);
            
            // ТЕСТ МАССИВА
            start = clock();

            while (!array_is_empty(&stack1))
            {
                array_pop(&stack1, &value);
                array_push(&temp, value);
            }
            while (!array_is_empty(&stack2))
            {
                array_pop(&stack2, &value);
                array_push(&temp, value);
            }
            
            while (!array_is_empty(&temp))
            {
                array_pop(&temp, &value);
                while (!array_is_empty(&result) && result.data[result.top] < value)
                {
                    array_pop(&result, &temp_val);
                    array_push(&temp, temp_val);
                }
                array_push(&result, value);
            }
            
            end = clock();
            total_array_time += (double)(end - start) / CLOCKS_PER_SEC;

            //printf("Отсортированный массив: \n");
            //array_display(&result);

            // ТЕСТ СПИСКА
            start = clock();
            
            while (!list_is_empty(&list_stack1))
            {
                list_pop(&list_stack1, &value);
                list_push(&list_temp, value);
            }
            while (!list_is_empty(&list_stack2))
            {
                list_pop(&list_stack2, &value);
                list_push(&list_temp, value);
            }
            
            while (!list_is_empty(&list_temp))
            {
                list_pop(&list_temp, &value);
                while (!list_is_empty(&list_result) && list_result.top->data < value)
                {
                    list_pop(&list_result, &temp_val);
                    list_push(&list_temp, temp_val);
                }
                list_push(&list_result, value);
            }

            end = clock();
            total_list_time += (double)(end - start) / CLOCKS_PER_SEC;

            //printf("Отсортированный список: \n");
            //list_display(&list_result);
            
            // Очистка списков
            list_free(&list_stack1);
            list_free(&list_stack2);
            list_free(&list_temp);
            list_free(&list_result);
            clear_free_addresses();
        }
        
        // Усреднение и вывод
        avg_array_time = total_array_time / NUM_RUNS;
        avg_list_time = total_list_time / NUM_RUNS;
        array_memory = 4 * n * sizeof(int);
        list_memory = 4 * n * sizeof(Node);
        time_ratio = (avg_list_time > 1e-9) ? avg_array_time / avg_list_time : 0;
        memory_ratio = (list_memory > 0) ? (double)array_memory / list_memory : 0;
        
        printf("    | %6d | %-11.6f | %-11.6f | %-10.6f | %-14lu | %-14lu | %-11.3f |\n", 
               n, avg_array_time, avg_list_time, time_ratio,
               (unsigned long)array_memory, 
               (unsigned long)list_memory,
               memory_ratio);
    } 
    printf("    +--------+-------------+-------------+------------+----------------+----------------+-------------+\n");
}

// Вспомогательные функции
int get_integer_input(const char* prompt, int min, int max)
{
    int value;
    int valid_input = 0;
    
    while (!valid_input) {
        printf("%s (%d <--> %d): ", prompt, min, max);
        
        if (scanf("%d", &value) != 1) {
            printf("Ошибка: введите целое число!\n");
            clear_input_buffer();
            continue;
        }
        
        if (value < min || value > max) {
            printf("Ошибка: число должно быть в диапазоне %d <--> %d!\n", min, max);
            continue;
        }
        
        valid_input = 1;
    }
    
    clear_input_buffer();
    return value;
}

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}