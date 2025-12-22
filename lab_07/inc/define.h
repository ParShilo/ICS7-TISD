#ifndef DEFINE_H__
#define DEFINE_H__

#define MAX_STRING_SIZE 10000

// BST дерево
typedef struct node_t 
{
    char data;
    int count;
    struct node_t *left, *right;
}  bst_node_t;

// AVL дерево
typedef struct avl_node_t
{
    char data;
    int count;
    int height;
    struct avl_node_t *left, *right;
} avl_node_t;

// Элемент хэш-таблицы с цепочками
typedef struct ht_chain_node
{
    char key;
    int count;
    struct ht_chain_node *next;
} ht_chain_node_t;

// Хэш-таблица с цепочками
typedef struct
{
    ht_chain_node_t **table;
    int size;
    int num_elements; 
} hashtable_chain_t;

// Статус элемента хэш-таблицы с открытой адресацией
typedef enum
{
    EMPTY,
    OCCUPIED,
    DELETED
} cell_status_t;

// Элемент хэш-таблицы с открытой адресацией
typedef struct
{
    char key;
    int count;
    cell_status_t status;
} ht_open_entry_t;

// Хэш-таблица с открытой адресацией
typedef struct
{
    ht_open_entry_t *table;
    int size;
    int num_elements;
} hashtable_open_t;

#endif