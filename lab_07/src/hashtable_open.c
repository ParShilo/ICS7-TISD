#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "errors.h"
#include "../inc/hashtable_open.h"

static int is_prime(int n)
{
    if (n < 2)
        return 0;

    if (n == 2)
        return 1;

    if (n % 2 == 0)
        return 0;

    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return 0;

    return 1;
}

static int next_prime(int n)
{
    if (n <= 2)
        return 2;

    if (n % 2 == 0)
        n++;

    while (is_prime(n) == 0)
        n += 2;

    return n;
}

static unsigned int hash_func(char key, int size)
{
    return ((unsigned char)key) % (unsigned int)size;
}

hashtable_open_t *ht_open_create(int initial_size)
{
    if (initial_size <= 0) return NULL;
    hashtable_open_t *ht = malloc(sizeof(hashtable_open_t));
    if (ht == NULL)
        return NULL;

    ht->size = next_prime(initial_size);
    ht->num_elements = 0;
    ht->table = calloc(ht->size, sizeof(ht_open_entry_t));
    if (ht->table == NULL)
    {
        free(ht);
        return NULL;
    }

    // Инициализация всех ячеек как EMPTY
    for (int i = 0; i < ht->size; i++)
        ht->table[i].status = EMPTY;

    return ht;
}

void ht_open_destroy(hashtable_open_t *ht)
{
    if (ht == NULL)
        return;
    free(ht->table);
    free(ht);
}

int ht_open_insert(hashtable_open_t *ht, char key)
{
    if (ht == NULL)
        return 0;
    unsigned int h = hash_func(key, ht->size);

    // Ищем первую EMPTY или DELETED ячейку
    for (int i = 0; i < ht->size; i++)
    {
        int idx = (h + i) % ht->size;
        if (ht->table[idx].status == EMPTY || ht->table[idx].status == DELETED)
        {
            ht->table[idx].key = key;
            ht->table[idx].count = 1;
            ht->table[idx].status = OCCUPIED;
            ht->num_elements++;
            return 1;
        }
        if (ht->table[idx].status == OCCUPIED && ht->table[idx].key == key)
        {
            ht->table[idx].count++;
            return 1;
        }
    }
    return 0; // таблица заполнена
}

int ht_open_search(hashtable_open_t *ht, char key, int *comparisons)
{
    if (!ht || !comparisons) 
        return -1;

    *comparisons = 0;
    unsigned int h = hash_func(key, ht->size);

    for (int i = 0; i < ht->size; i++)
    {
        int idx = (h + i) % ht->size;
        (*comparisons)++;

        if (ht->table[idx].status == EMPTY)
            break; // ключ точно отсутствует

        if (ht->table[idx].status == OCCUPIED && ht->table[idx].key == key)
            return ht->table[idx].count;
        // если DELETED — продолжаем поиск
    }
    return -1;
}

int ht_open_delete(hashtable_open_t *ht, char key)
{
    if (!ht) return 0;
    unsigned int h = hash_func(key, ht->size);

    for (int i = 0; i < ht->size; i++)
    {
        int idx = (h + i) % ht->size;
        if (ht->table[idx].status == EMPTY)
            return 0; // не найден

        if (ht->table[idx].status == OCCUPIED && ht->table[idx].key == key)
        {
            ht->table[idx].status = DELETED;
            ht->num_elements--;
            return 1;
        }
        // DELETED — пропускаем, продолжаем
    }
    return 0;
}

void ht_open_print(hashtable_open_t *ht)
{
    if (!ht)
    {
        printf("Хеш-таблица (адресация) не инициализирована.\n");
        return;
    }

    printf("\nХеш-таблица (линейная адресация), размер = %d, элементов = %d:\n", ht->size, ht->num_elements);
    printf("+------+------------------+\n");
    printf("| Инд. | Содержимое       |\n");
    printf("+------+------------------+\n");

    for (int i = 0; i < ht->size; i++)
    {
        printf("| %4d |", i);
        if (ht->table[i].status == EMPTY)
            printf(" ---              |\n");
        else if (ht->table[i].status == DELETED)
            printf(" DEL              |\n");
        else
            printf(" '%c'(%3d)         |\n", ht->table[i].key, ht->table[i].count);
    }
    printf("+------+------------------+\n");
}

hashtable_open_t *ht_open_rehash(hashtable_open_t *ht)
{
    if (ht == NULL)
        return NULL;

    int new_size = next_prime((int)ceil(ht->num_elements * 1.2));
    if (new_size <= ht->size)
        new_size = next_prime(ht->size * 2);

    hashtable_open_t *new_ht = ht_open_create(new_size);
    if (new_ht == NULL)
        return NULL;

    // Переносим только OCCUPIED
    for (int i = 0; i < ht->size; i++)
    {
        if (ht->table[i].status == OCCUPIED)
        {
            ht_open_insert(new_ht, ht->table[i].key);
            // Восстанавливаем count
            unsigned int h = hash_func(ht->table[i].key, new_ht->size);
            for (int j = 0; j < new_ht->size; j++)
            {
                int idx = (h + j) % new_ht->size;
                if (new_ht->table[idx].status == OCCUPIED && new_ht->table[idx].key == ht->table[i].key)
                {
                    new_ht->table[idx].count = ht->table[i].count;
                    break;
                }
            }
        }
    }

    ht_open_destroy(ht);
    return new_ht;
}

double ht_open_avg_comparisons(hashtable_open_t *ht)
{
    if (ht == NULL || ht->num_elements == 0) return 0.0;

    int total_comparisons = 0;
    int count = 0;

    for (int i = 0; i < ht->size; i++)
    {
        if (ht->table[i].status == OCCUPIED)
        {
            int cmp = 0;
            ht_open_search(ht, ht->table[i].key, &cmp);
            total_comparisons += cmp;
            count++;
        }
    }

    return count > 0 ? (double)total_comparisons / count : 0.0;
}