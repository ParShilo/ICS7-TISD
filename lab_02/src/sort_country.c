#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "errors.h"
#include "country.h"
#include "print.h"
#include "proc_country.h"
#include "sort_country.h"
#include "key.h"

// Пузырьковая сортировка исходной таблицы
void bubble_sort(country_t countries[], const size_t number_countries)
{
    country_t temp;

    for (size_t i = 0; i < number_countries - 1; i++)
    {
        for (size_t j = 0; j < number_countries - i - 1; j++)
        {
            if (countries[j].min_cost > countries[j + 1].min_cost)
            {
                temp = countries[j];
                countries[j] = countries[j + 1];
                countries[j + 1] = temp;
            }
        }
    }
}

// Быстрая сортировка исходной таблицы
void quick_sort(country_t countries[], int low, int high)
{
    country_t temp;

    if (low < high)
    {
        int pivot = countries[(low + high) / 2].min_cost;
        int i = low, j = high;
        
        while (i <= j)
        {
            while (countries[i].min_cost < pivot)
                i++;
            while (countries[j].min_cost > pivot) 
                j--;
            if (i <= j)
            {
                temp = countries[i];
                countries[i] = countries[j];
                countries[j] = temp;
                i++;
                j--;
            }
        }
        
        quick_sort(countries, low, j);
        quick_sort(countries, i, high);
    }
}

// Тестирование алгоритмов сортировки
void test_sorting(const country_t countries[], const size_t number_countries)
{
    if (number_countries == 0)
    {
        printf("Нет данных для тестирования.\n");
        return;
    }
    
    printf("Размер таблицы: %zu записей\n", number_countries);
    printf("Измерение времени для 5 запусков каждого алгоритма...\n");
    
    clock_t start, end;
    double time_original_bubble = 0, time_original_quick = 0;
    double time_keys_bubble = 0, time_keys_quick = 0;
    
    country_t countries_1[MAX_COUNTRIES];
    country_t countries_2[MAX_COUNTRIES];
    key_t key_table_1[MAX_COUNTRIES];
    key_t key_table_2[MAX_COUNTRIES];

    for (int run = 0; run < 5; run++)
    {
        memcpy(countries_1, countries, number_countries * sizeof(country_t));
        memcpy(countries_2, countries, number_countries * sizeof(country_t));
        
        // 1. Пузырьковая сортировка исходной таблицы
        start = clock();
        bubble_sort(countries_1, number_countries);
        end = clock();
        time_original_bubble += (double)(end - start) / CLOCKS_PER_SEC;
        
        // 2. Быстрая сортировка исходной таблицы
        start = clock();
        quick_sort(countries_2, 0, number_countries - 1);
        end = clock();
        time_original_quick += (double)(end - start) / CLOCKS_PER_SEC;
        
        // 3. Пузырьковая сортировка ключей
        start = clock();
        initialize_key_table(key_table_1, countries, number_countries);
        bubble_sort_key(key_table_1, number_countries);
        end = clock();
        time_keys_bubble += (double)(end - start) / CLOCKS_PER_SEC;
        
        // 4. Быстрая сортировка ключей
        start = clock();
        initialize_key_table(key_table_2, countries, number_countries);
        quick_sort_key(key_table_2, 0, number_countries - 1);
        end = clock();
        time_keys_quick += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    time_original_bubble /= 5;
    time_original_quick /= 5;
    time_keys_bubble /= 5;
    time_keys_quick /= 5;
    
    // Вывод результатов
    printf("\n+-------------------------------------------------------------+\n");
    printf("| Алгоритм сортировки           | Время (сек.) | Эффективность|\n");
    printf("+-------------------------------------------------------------+\n");
    printf("| Исходная таблица (пузырьком)  | %-12.6f |    100%%     |\n", time_original_bubble);
    printf("| Исходная таблица (быстрая)    | %-12.6f |    %-3.0f%%     |\n", 
           time_original_quick, (time_original_bubble/time_original_quick)*100);
    printf("| Ключи (пузырьком)             | %-12.6f |    %-3.0f%%     |\n", 
           time_keys_bubble, (time_original_bubble/time_keys_bubble)*100);
    printf("| Ключи (быстрая)               | %-12.6f |    %-3.0f%%     |\n", 
           time_keys_quick, (time_original_bubble/time_keys_quick)*100);
    printf("+-------------------------------------------------------------+\n");
    
    // Расчет памяти
    size_t mem_original = number_countries * sizeof(country_t);
    size_t mem_keys = number_countries * sizeof(key_t);
    size_t mem_total_with_keys = mem_original + mem_keys;
    
    printf("\nИспользование памяти:\n");
    printf("+-----------------------------------+\n");
    printf("| Структура данных   | Память (байт)|\n");
    printf("+-----------------------------------+\n");
    printf("| Исходная таблица   | %-13zu|\n", mem_original);
    printf("| Таблица ключей     | %-13zu|\n", mem_keys);
    printf("| Общая с ключами    | %-13zu|\n", mem_total_with_keys);
    printf("+-----------------------------------+\n");
}