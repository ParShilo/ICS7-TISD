#ifndef AVL_NODE_H__
#define AVL_NODE_H__

#include "define.h"

// Основные операции
avl_node_t *avl_create_node(char data);
avl_node_t *avl_insert(avl_node_t *root, char data);
avl_node_t *avl_delete(avl_node_t *root, char key);
avl_node_t *avl_search(avl_node_t *root, char key);
avl_node_t *avl_search_with_cmp(avl_node_t *root, char key, int *comparisons);

// Вспомогательные
int avl_height(const avl_node_t *node);
int avl_get_balance(const avl_node_t *node);
avl_node_t *avl_rotate_right(avl_node_t *y);
avl_node_t *avl_rotate_left(avl_node_t *x);
avl_node_t *avl_find_min(avl_node_t *node);

// Освобождение и анализ
void avl_free(avl_node_t *root);
size_t avl_memory_usage(const avl_node_t *root);
int avl_tree_height(const avl_node_t *root);

// Вывод
int avl_create_dot(avl_node_t *root, const char *filename, int highlight_duplicates);
void avl_print_to_file(avl_node_t *root, FILE *f, int depth);

#endif