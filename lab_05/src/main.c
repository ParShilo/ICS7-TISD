#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "defines.h"
#include "print.h"
#include "arr.h"
#include "list.h"
#include "simulate.h"

void clear_input_buffer(void);

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
                    request_t temp = {.type=value, .arrival_time=generate_random_time(q1_arrival_min, q1_arrival_max)};
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
                rc = simulate_array_queue(1);
                break;
            }
            // Добавить элемент для списка-очереди
            case 5:
            {
                printf("Введите элемент для добавления в список-очередь: ");
                if (scanf("%d", &value) == 1)
                {
                    printf("Показывать адрес? (1 - да, 0 - нет): ");
                    if (scanf("%d", &show_addr) == 1)
                        rc = enqueue_list(&list_q, value, 1.0, show_addr);
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
                printf("Показывать адрес? (1 - да, 0 - нет): ");
                if (scanf("%d", &show_addr) == 1)
                    rc = dequeue_list(&list_q, show_addr);
                else
                    rc = ERROR_IO;
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
                //emulate_queue_list();
                printf("Моделирование для очереди-списка (функция в разработке)\n");
                break;
            }
            // Сравнение производительности
            case 10:
            {
                //compare_queues();
                printf("Сравнение производительности (функция в разработке)\n");
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
    while (is_list_queue_empty(&list_q) == ERROR_OK)
        dequeue_list(&list_q, 0);
    
    return rc;
}

// Функция очистки буфера
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}