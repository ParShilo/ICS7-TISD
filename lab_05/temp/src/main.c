#include "array.h"
#include "list.h"
#include "io.h"
#include "err.h"
#include "structs.h"
#include <stdbool.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));
    int choice;
    mem_t *mem = NULL;
    int mem_used = 0;
    
    printf("Лабораторная работа №5: Моделирование очереди\n");
    printf("Вариант: Заявки двух типов с возвратом на 4-ю позицию\n");
    
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода!\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: {
                unsigned long time = simulate_array(&mem_used, false);
                printf("Время выполнения (массив): %lu тиков\n", time);
                break;
            }
            case 2: {
                unsigned long time = simulate_list(&mem, &mem_used, false);
                printf("Время выполнения (список): %lu тиков\n", time);
                break;
            }
            case 3:
                show_mem(&mem);
                break;
            case 4: {
                int mem_array, mem_list;
                unsigned long time_array = simulate_array(&mem_array, false);
                unsigned long time_list = simulate_list(&mem, &mem_list, false);
                
                printf("\n=== СРАВНИТЕЛЬНЫЙ АНАЛИЗ ===\n");
                printf("Массив:  время=%lu тиков, память=%d байт\n", time_array, mem_array);
                printf("Список:  время=%lu тиков, память=%d байт\n", time_list, mem_list);
                printf("Отношение список/массив: время=%.2f, память=%.2f\n",
                       (double)time_list/time_array, (double)mem_list/mem_array);
                break;
            }
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
    } while (choice != 0);
    
    return ERROR_OK;
}