#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "simulation.h"

int main() {
    srand(time(NULL));
    int choice;
    mem_t *memory_tracker = NULL;
    
    print_welcome();
    
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода!\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("\n--- МОДЕЛИРОВАНИЕ С ОЧЕРЕДЬЮ-МАССИВОМ ---\n");
                int mem_array;
                simulate_array_queue(&mem_array, true);
                break;
                
            case 2:
                printf("\n--- МОДЕЛИРОВАНИЕ С ОЧЕРЕДЬЮ-СПИСКОМ ---\n");
                int mem_list;
                simulate_list_queue(&mem_list, &memory_tracker, true);
                break;
                
            case 3:
                show_memory_addresses(memory_tracker);
                break;
                
            case 4:
                compare_queues();
                break;
                
            case 0:
                printf("Выход из программы...\n");
                break;
                
            default:
                printf("Неверный выбор!\n");
        }
    } while (choice != 0);
    
    // Очистка памяти
    mem_t *current = memory_tracker;
    while (current != NULL) {
        mem_t *temp = current;
        current = current->next;
        free(temp);
    }
    
    return 0;
}