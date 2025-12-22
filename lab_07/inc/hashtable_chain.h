#ifndef HASHTABLE_CHAIN_H__
#define HASHTABLE_CHAIN_H__

#include <stdio.h>
#include "../inc/define.h"

// Вспомогательные
int next_prime(int n);

// Основные функции
hashtable_chain_t *ht_chain_create(int initial_size);
void ht_chain_destroy(hashtable_chain_t *ht);
int ht_chain_insert(hashtable_chain_t *ht, char key);
int ht_chain_search(hashtable_chain_t *ht, char key, int *comparisons);
int ht_chain_delete(hashtable_chain_t *ht, char key);
void ht_chain_print(hashtable_chain_t *ht);
hashtable_chain_t *ht_chain_rehash(hashtable_chain_t *ht);

// Анализ
double ht_chain_avg_comparisons(hashtable_chain_t *ht);

#endif