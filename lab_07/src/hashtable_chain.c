#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "errors.h"
#include "../inc/hashtable_chain.h"

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

int next_prime(int n)
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

hashtable_chain_t *ht_chain_create(int initial_size)
{
    if (initial_size <= 0)
        return NULL;

    hashtable_chain_t *ht = malloc(sizeof(hashtable_chain_t));
    if (ht == NULL)
        return NULL;

    ht->size = next_prime(initial_size);
    ht->num_elements = 0;
    ht->table = calloc(ht->size, sizeof(ht_chain_node_t*));
    if (ht->table == NULL)
    {
        free(ht);
        return NULL;
    }
    return ht;
}

void ht_chain_destroy(hashtable_chain_t *ht)
{
    if (ht == NULL)
        return;
    
    for (int i = 0; i < ht->size; i++)
    {
        ht_chain_node_t *node = ht->table[i], *tmp;
        while (node)
        {
            tmp = node;
            node = node->next;
            free(tmp);
        }
    }

    free(ht->table);
    free(ht);
}

int ht_chain_insert(hashtable_chain_t *ht, char key)
{
    if (ht == NULL)
        return 0;

    unsigned int idx = hash_func(key, ht->size);
    ht_chain_node_t *head = ht->table[idx];

    
    while (head)
    {
        if (head->key == key)
        {
            head->count++;
            return 1;
        }
        head = head->next;
    }

    
    ht_chain_node_t *new_node = malloc(sizeof(ht_chain_node_t));
    if (new_node == NULL)
        return 0;

    new_node->key = key;
    new_node->count = 1;
    new_node->next = ht->table[idx];
    ht->table[idx] = new_node;
    ht->num_elements++;
    return 1;
}

int ht_chain_search(hashtable_chain_t *ht, char key, int *comparisons)
{
    if (ht == NULL || comparisons == NULL)
        return -1;

    *comparisons = 0;
    unsigned int idx = hash_func(key, ht->size);
    ht_chain_node_t *node = ht->table[idx];

    while (node)
    {
        (*comparisons)++;
        if (node->key == key)
            return node->count;
        node = node->next;
    }
    return -1;
}

int ht_chain_delete(hashtable_chain_t *ht, char key)
{
    if (ht == NULL)
        return 0;
    unsigned int idx = hash_func(key, ht->size);
    ht_chain_node_t **pp = &(ht->table[idx]);

    while (*pp)
    {
        if ((*pp)->key == key)
        {
            ht_chain_node_t *to_del = *pp;
            *pp = (*pp)->next;
            free(to_del);
            ht->num_elements--;
            return 1;
        }
        pp = &((*pp)->next);
    }
    return 0;
}

void ht_chain_print(hashtable_chain_t *ht)
{
    if (ht  == NULL)
    {
        printf("Хеш-таблица не инициализирована.\n");
        return;
    }

    printf("\nХеш-таблица (цепочки), размер = %d, элементов = %d:\n", ht->size, ht->num_elements);
    printf("+------+------------------+\n");
    printf("| Инд. | Содержимое       |\n");
    printf("+------+------------------+\n");

    for (int i = 0; i < ht->size; i++)
    {
        printf("| %4d |", i);
        ht_chain_node_t *node = ht->table[i];
        if (node == NULL)
            printf(" ---\n");
        else
        {
            printf(" ");
            while (node)
            {
                printf("'%c'(%d)", node->key, node->count);
                node = node->next;
                if (node)
                    printf(" → ");
            }
            printf("\n");
        }
    }
    printf("+------+------------------+\n");
}

hashtable_chain_t *ht_chain_rehash(hashtable_chain_t *ht)
{
    if (ht == NULL)
        return NULL;

    int new_size = next_prime((int)ceil(ht->num_elements / 0.72));
    if (new_size <= ht->size)
        new_size = next_prime(ht->size * 2);
    hashtable_chain_t *new_ht = ht_chain_create(new_size);
    if (new_ht == NULL)
        return NULL;

    
    for (int i = 0; i < ht->size; i++)
    {
        ht_chain_node_t *node = ht->table[i];
        while (node)
        {
            ht_chain_insert(new_ht, node->key);
            unsigned int idx = hash_func(node->key, new_ht->size);
            ht_chain_node_t *n = new_ht->table[idx];
            while (n && n->key != node->key)
                n = n->next;
            if (n)
                n->count = node->count;
            node = node->next;
        }
    }

    ht_chain_destroy(ht);
    return new_ht;
}

double ht_chain_avg_comparisons(hashtable_chain_t *ht)
{
    if (ht == NULL || ht->num_elements == 0)
        return 0.0;

    int total_comparisons = 0;
    int total_keys = 0;

    for (int i = 0; i < ht->size; i++)
    {
        ht_chain_node_t *node = ht->table[i];
        int pos = 1;
        while (node)
        {
            total_comparisons += pos; 
            total_keys++;
            pos++;
            node = node->next;
        }
    }

    return total_keys > 0 ? (double)total_comparisons / total_keys : 0.0;
}