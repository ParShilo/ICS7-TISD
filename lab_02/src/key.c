#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "country.h"
#include "print.h"
#include "key.h"

// Инициализация таблицы ключей
void initialize_key_table(key_t key_table[], const country_t countries[], const size_t number_countries)
{
    for (size_t i = 0; i < number_countries; i++)
    {
        key_table[i].index = i;
        key_table[i].key = countries[i].min_cost;
    }
}

// Пузырьковая сортировка таблицы ключей
void bubble_sort_key(key_t key_table[], const size_t number_countries)
{
    key_t temp;

    for (size_t i = 0; i < number_countries - 1; i++)
    {
        for (size_t j = 0; j < number_countries - i - 1; j++)
        {
            if (key_table[j].key > key_table[j + 1].key)
            {
                temp = key_table[j];
                key_table[j] = key_table[j + 1];
                key_table[j + 1] = temp;
            }
        }
    }
}

// Быстрая сортировка таблицы ключей
void quick_sort_key(key_t key_table[], int low, int high)
{
    key_t temp;

    if (low < high)
    {
        int pivot = key_table[(low + high) / 2].key;
        int i = low, j = high;
        
        while (i <= j)
        {
            while (key_table[i].key < pivot) i++;
            while (key_table[j].key > pivot) j--;
            if (i <= j)
            {
                temp = key_table[i];
                key_table[i] = key_table[j];
                key_table[j] = temp;
                i++;
                j--;
            }
        }
        
        quick_sort_key(key_table, low, j);
        quick_sort_key(key_table, i, high);
    }
}