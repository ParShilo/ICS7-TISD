#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "errors.h"
#include "defines.h"
#include "print.h"
#include "arr.h"
#include "list.h"
#include "simulate.h"

void clear_input_buffer(void);
int update_simulation_parameters(void);

int main(void)
{
    int choice, value, rc = ERROR_OK;
    int show_addr;
    array_queue arr_q;
    list_queue list_q;
    
    // Инициализация структур
    init_array_queue(&arr_q);
    init_list_queue(&list_q);
    
    printf("Программа работы с очередью.\n");
    
    do {
        // Вывод меню
        print_menu();
        if (scanf("%d", &choice) != 1)
        {
            rc = ERROR_IO;
            break;
        }
        
        switch (choice)
        {
            // Добавить элемент для массива-очереди
            case 1:
            {
                printf("Введите элемент для добавления в массив-очередь: ");
                if (scanf("%d", &value) == 1)
                {
                    request_t temp = {.type = value, .arrival_time = 1.0};
                    rc = enqueue_array(&arr_q, temp, 1);
                }
                else
                    rc = ERROR_IO;
                break;
            }
            // Удалить элемент для массива-очереди
            case 2:
            {
                if (is_array_queue_empty(&arr_q))
                    rc = ERROR_EMPTY;
                else
                    dequeue_array(&arr_q, 1);
                break;
            }
            // Вывести массив-очередь
            case 3:
            {
                print_array_queue(&arr_q);
                break;
            }
            // Моделирование массива-очереди
            case 4:
            {
                srand(time(NULL));
                rc = simulate_array_queue(1, NULL);
                break;
            }
            // Добавить элемент для списка-очереди
            case 5:
            {
                printf("Показывать адрес? (1 - да, 0 - нет): ");
                if (scanf("%d", &show_addr) == 1 || show_addr == 0 || show_addr == 1)
                {
                    printf("Введите элемент для добавления в список-очередь: ");
                    if (scanf("%d", &value) == 1)
                    {
                        request_t temp = {.type = value, .arrival_time = 1.0};
                        rc = enqueue_list(&list_q, temp, show_addr);
                    }
                    else
                        rc = ERROR_IO;
                }
                else 
                    rc = ERROR_IO;
                break;
            }
            // Удалить элемент для списка-очереди
            case 6:
            {
                if (is_list_queue_empty(&list_q))
                    rc = ERROR_EMPTY;
                else
                {
                    printf("Показывать адрес? (1 - да, 0 - нет): ");
                    if (scanf("%d", &show_addr) == 1)
                        dequeue_list(&list_q, show_addr);
                    else
                        rc = ERROR_IO;
                }
                break;
            }
            // Вывести список-очередь
            case 7:
            {
                print_list_queue(&list_q);
                break;
            }
            // Вывести адреса элементов списка-очереди
            case 8:
            {
                print_list_addresses();
                break;
            }
            // Моделирование списка-очереди
            case 9:
            {
                srand(time(NULL));
                rc = simulate_list_queue(1, NULL);
                break;
            }
            // Сравнение производительности
            case 10:
            {
                rc = compare_queues();
                break;
            }
            // Изменение входных параметров
            case 11:
            {
                rc = update_simulation_parameters();
                break;
            }
            // Выход
            case 0:
            {
                printf("Выход из программы.\n");
                break;
            }
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
        }
        
        // Очистка буфера ввода после каждой операции
        clear_input_buffer();
        
    } while (rc == ERROR_OK && choice != 0);
    
    // Очистка памяти для списка
    while (!is_list_queue_empty(&list_q))
        dequeue_list(&list_q, 0);
    
    if (rc != ERROR_OK)
        print_error(rc);

    return rc;
}

// Функция очистки буфера
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Функция изменения входных данных
int update_simulation_parameters(void)
{
    double min, max;

    // Приход заявок 1 типа
    printf("Введите min и max для интервала прихода заявок 1 типа (через пробел): ");
    if ((scanf("%lf %lf", &min, &max) != 2) || (min < 0) || (max < min))
        return ERROR_IO;
    q1_arrival_min = min;
    q1_arrival_max = max;

    // Обслуживание заявок 1 типа
    printf("Введите min и max для времени обслуживания заявок 1 типа: ");
    if ((scanf("%lf %lf", &min, &max) != 2) || (min < 0) || (max < min))
        return ERROR_IO;
    q1_service_min = min;
    q1_service_max = max;

    // Обслуживание заявок 2 типа
    printf("Введите min и max для времени обслуживания заявок 2 типа: ");
    if ((scanf("%lf %lf", &min, &max) != 2) || (min < 0) || (max < min))
        return ERROR_IO;
    q2_service_min = min;
    q2_service_max = max;

    printf("Параметры успешно обновлены!\n");
    printf("  Приход 1 типа: [%.2f, %.2f]\n", q1_arrival_min, q1_arrival_max);
    printf("  Обсл. 1 типа: [%.2f, %.2f]\n", q1_service_min, q1_service_max);
    printf("  Обсл. 2 типа: [%.2f, %.2f]\n", q2_service_min, q2_service_max);

    return ERROR_OK;
}