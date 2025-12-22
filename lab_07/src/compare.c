// src/compare.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../inc/define.h"
#include "../inc/bst_node.h"
#include "../inc/avl_node.h"
#include "../inc/hashtable_chain.h"
#include "../inc/hashtable_open.h"
#include "../inc/compare.h"

static void generate_random_string(char *buf, int size)
{
    const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand((unsigned int)time(NULL));
    for (int i = 0; i < size - 1; i++)
        buf[i] = letters[rand() % 52];
    buf[size - 1] = '\0';
}

static char *get_unique_chars(const char *str, int *n_unique)
{
    int seen[256] = {0};
    *n_unique = 0;
    for (int i = 0; str[i]; i++)
        if (!seen[(unsigned char)str[i]])
            seen[(unsigned char)str[i]] = ++(*n_unique);

    char *uniq = malloc(*n_unique * sizeof(char));
    if (!uniq) return NULL;
    int j = 0;
    for (int i = 0; i < 256; i++)
        if (seen[i]) uniq[j++] = (char)i;
    return uniq;
}

void compare_all_structures(void)
{
    const int STR_SIZE = 10000;
    const int RUNS = 100000;
    char input[STR_SIZE + 1];
    
    printf("Генерация строки из %d случайных букв...\n", STR_SIZE);
    generate_random_string(input, STR_SIZE);
    
    // Построение структур
    bst_node_t *bst = NULL;
    avl_node_t *avl = NULL;
    for (int i = 0; input[i]; i++)
    {
        bst = bst_insert(bst, input[i]);
        avl = avl_insert(avl, input[i]);
    }
    
    int n_unique = 0;
    char *uniq = get_unique_chars(input, &n_unique);
    if (!uniq)
    {
        printf("Ошибка выделения памяти!\n");
        bst_free(bst);
        avl_free(avl);
        return;
    }
    
    // Хеш-таблицы
    int size_chain = (int)ceil(n_unique / 0.72);
    hashtable_chain_t *ht_chain = ht_chain_create(size_chain);
    int size_open = (int)ceil(n_unique * 1.2);
    hashtable_open_t *ht_open = ht_open_create(size_open);
    
    for (int i = 0; input[i]; i++)
    {
        ht_chain_insert(ht_chain, input[i]);
        ht_open_insert(ht_open, input[i]);
    }
    
    // === ЗАМЕРЫ ===
    clock_t start, end;
    double time_bst = 0, time_avl = 0, time_chain = 0, time_open = 0;
    long total_cmp_bst = 0, total_cmp_avl = 0, total_cmp_chain = 0, total_cmp_open = 0;
    
    // BST
    for (int run = 0; run < RUNS; run++)
    {
        int cmp = 0;
        start = clock();
        for (int i = 0; i < n_unique; i++)
        {
            int c = 0;
            bst_search_with_cmp(bst, uniq[i], &c);
            cmp += c;
        }
        end = clock();
        time_bst += (double)(end - start);
        total_cmp_bst += cmp;
    }
    
    // AVL
    for (int run = 0; run < RUNS; run++)
    {
        int cmp = 0;
        start = clock();
        for (int i = 0; i < n_unique; i++)
        {
            int c = 0;
            avl_search_with_cmp(avl, uniq[i], &c);
            cmp += c;
        }
        end = clock();
        time_avl += (double)(end - start);
        total_cmp_avl += cmp;
    }
    
    // Хеш цепочки
    for (int run = 0; run < RUNS; run++)
    {
        int cmp = 0;
        start = clock();
        for (int i = 0; i < n_unique; i++)
        {
            int c = 0;
            ht_chain_search(ht_chain, uniq[i], &c);
            cmp += c;
        }
        end = clock();
        time_chain += (double)(end - start);
        total_cmp_chain += cmp;
    }
    
    // Хеш адресация
    for (int run = 0; run < RUNS; run++)
    {
        int cmp = 0;
        start = clock();
        for (int i = 0; i < n_unique; i++)
        {
            int c = 0;
            ht_open_search(ht_open, uniq[i], &c);
            cmp += c;
        }
        end = clock();
        time_open += (double)(end - start);
        total_cmp_open += cmp;
    }
    
    // Перевод в микросекунды и усреднение
    time_bst = (time_bst / RUNS) * 1000000.0 / CLOCKS_PER_SEC;
    time_avl = (time_avl / RUNS) * 1000000.0 / CLOCKS_PER_SEC;
    time_chain = (time_chain / RUNS) * 1000000.0 / CLOCKS_PER_SEC;
    time_open = (time_open / RUNS) * 1000000.0 / CLOCKS_PER_SEC;
    
    double avg_cmp_bst = (double)total_cmp_bst / (RUNS * n_unique);
    double avg_cmp_avl = (double)total_cmp_avl / (RUNS * n_unique);
    double avg_cmp_chain = (double)total_cmp_chain / (RUNS * n_unique);
    double avg_cmp_open = (double)total_cmp_open / (RUNS * n_unique);
    
    // Память
    size_t mem_bst = bst_memory_usage(bst);
    size_t mem_avl = avl_memory_usage(avl);
    size_t mem_chain = ht_chain->size * sizeof(ht_chain_node_t*) + 
                       ht_chain->num_elements * sizeof(ht_chain_node_t);
    size_t mem_open = ht_open->size * sizeof(ht_open_entry_t);
    
    // === ВЫВОД ===
    printf("\n");
    printf("============================================================================================================\n");
    printf("                                        СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ СТРУКТУР ДАННЫХ\n");
    printf("============================================================================================================\n");
    printf("Входные данные: строка из %d случайных строчных букв, уникальных символов: %d\n", STR_SIZE, n_unique);
    printf("Количество прогонов: %d\n", RUNS);
    printf("\n");
    printf("+---------------------+------------------+-----------------+--------------------+------------------+\n");
    printf("| Структура данных    | Время (мкс)      | Память (байт)   | Всего сравнений    | Ср. сравнений    |\n");
    printf("+---------------------+------------------+-----------------+--------------------+------------------+\n");
    printf("| BST                 | %16.3f | %15zu | %18ld | %16.3f |\n", 
           time_bst, mem_bst, total_cmp_bst / RUNS, avg_cmp_bst);
    printf("| AVL                 | %16.3f | %15zu | %18ld | %16.3f |\n", 
           time_avl, mem_avl, total_cmp_avl / RUNS, avg_cmp_avl);
    printf("| Хеш (цепочки)       | %16.3f | %15zu | %18ld | %16.3f |\n", 
           time_chain, mem_chain, total_cmp_chain / RUNS, avg_cmp_chain);
    printf("| Хеш (адресация)     | %16.3f | %15zu | %18ld | %16.3f |\n", 
           time_open, mem_open, total_cmp_open / RUNS, avg_cmp_open);
    printf("+---------------------+------------------+-----------------+--------------------+------------------+\n");
    
    // Очистка
    bst_free(bst);
    avl_free(avl);
    ht_chain_destroy(ht_chain);
    ht_open_destroy(ht_open);
    free(uniq);
}