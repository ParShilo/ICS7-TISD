#ifndef HASHTABLE_OPEN_H__
#define HASHTABLE_OPEN_H__

#include <stdio.h>
#include "../inc/define.h"

// Основные функции
hashtable_open_t *ht_open_create(int initial_size);
void ht_open_destroy(hashtable_open_t *ht);
int ht_open_insert(hashtable_open_t *ht, char key);
int ht_open_search(hashtable_open_t *ht, char key, int *comparisons);
int ht_open_delete(hashtable_open_t *ht, char key);
void ht_open_print(hashtable_open_t *ht);
hashtable_open_t *ht_open_rehash(hashtable_open_t *ht);
double ht_open_avg_comparisons(hashtable_open_t *ht);

#endif