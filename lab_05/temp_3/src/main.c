#include "simulate.h"
#include "efficiency.h"
#include <stdio.h>

float q1_arrival_min = 0, q1_arrival_max = 5;
float q1_service_min = 0, q1_service_max = 4;
float q2_service_min = 0, q2_service_max = 4;

void manual_mode();

int main(void) 
{
    int choice;
    int rc;
    char *menu = "| Пункт |               Действие                    |\n"
                "|-------|-------------------------------------------|\n"
                "|   0   | Выход                                     |\n"
                "|   1   | Запустить симуляцию для очереди массивом  |\n"
                "|   2   | Запустить симуляцию для очереди списком   |\n"
                "|   3   | Оценка эффективности                      |\n"
                "|   4   | Ручной режим                              |\n"
                "|   5   | Изменить параметры симуляции              |\n"
                "|---------------------------------------------------|\n";

    while (1) {
        printf("Выберите пункт меню:\n");
        printf("%s", menu);
        rc = scanf("%d", &choice);
        if (rc != 1) {
            printf("Некорректное значение ввода\n");
            while (getchar() != '\n');
            continue;
        } else {
            switch (choice) {
                case 0:
                    return 0;
                case 1:
                    simulate_arr(1);
                    break;
                case 2:
                    simulate_list(1);
                    break;
                case 3:
                    efficiency();
                    break;
                case 4:
                    manual_mode();
                    break;
                case 5:
                    printf("Введите интервал прихода заявок 1-го типа (min max): ");
                    scanf("%f%f", &q1_arrival_min, &q1_arrival_max);
                    printf("Введите время обслуживания заявок 1-го типа (min max): ");
                    scanf("%f%f", &q1_service_min, &q1_service_max);
                    printf("Введите время обслуживания заявки 2-го типа (min max): ");
                    scanf("%f%f", &q2_service_min, &q2_service_max);
                    break;
                default:
                    printf("Некорректное значение ввода\n");
                    break;
            }
        }
    }
    return 0;
}

void manual_mode()
{
    int choice;
    int rc;
    char *menu = "| Пункт |              Действие            |\n"
                "|-------|----------------------------------|\n"
                "|   1   | Добавить заявку в массив-очередь |\n"
                "|   2   | Добавить заявку в список-очередь |\n"
                "|   3   | Удалить заявку из массива-очереди|\n"
                "|   4   | Удалить заявку из списка-очереди |\n"
                "|   5   | Текущее состояние очередей       |\n"
                "|   0   | Выйти из меню                    |\n";

    arr_queue_t *arr_queue = create_arr();
    list_queue_t *list_queue = create_list();

    while (1)
    {
        printf("%s", menu);
        printf("Введите пункт меню:\n");
        rc = scanf("%d", &choice);
        if (rc != 1)
        {
            printf("Неверный пункт меню\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice)
        {
            case 1:
            {
                task_t *task = create_task(TYPE1);
                rc = push_arr(arr_queue, task);
                if (rc != 0)
                {
                    if (rc == OVERFLOW) printf("Очередь переполнена!\n");
                    else if (rc == ALLOCATE) printf("Ошибка выделения памяти\n");
                    delete_task(task);
                }
                else
                {
                    printf("Заявка добавлена. Адрес: %p\n", (void*)task);
                }
                break;
            }
            case 2:
            {
                task_t *task = create_task(TYPE1);
                rc = push_list(list_queue, task);
                if (rc != 0)
                {
                    if (rc == OVERFLOW) printf("Очередь переполнена!\n");
                    else if (rc == ALLOCATE) printf("Ошибка выделения памяти\n");
                    delete_task(task);
                }
                else
                {
                    printf("Заявка добавлена. Адрес: %p\n", (void*)task);
                }
                break;
            }
            case 3:
            {
                task_t *task = pop_arr(arr_queue);
                if (task == NULL) printf("Очередь пуста!\n");
                else
                {
                    printf("Заявка удалена. Адрес: %p, Тип: %d\n", (void*)task, task->type);
                    delete_task(task);
                }
                break;
            }
            case 4:
            {
                task_t *task = pop_list(list_queue);
                if (task == NULL) printf("Очередь пуста!\n");
                else
                {
                    printf("Заявка удалена. Адрес: %p, Тип: %d\n", (void*)task, task->type);
                    delete_task(task);
                }
                break;
            }
            case 5:
            {
                printf("Очередь массивом (количество: %d):\n", arr_queue->amount);
                print_arr(arr_queue);
                printf("Очередь списком (количество: %d):\n", list_queue->amount);
                print_list(list_queue);
                break;
            }
            case 0:
                delete_arr(arr_queue);
                delete_list(list_queue);
                return;
            default:
                printf("Неверный пункт меню\n");
                break;
        }
    }
}