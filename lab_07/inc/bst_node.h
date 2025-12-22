#ifndef BST_NODE_H__
#define BST_NODE_H__

#include "define.h"

// Основные операции
bst_node_t *bst_create_node(char data);
bst_node_t *bst_insert(bst_node_t *root, char data);
bst_node_t *bst_search(bst_node_t *root, char key);
bst_node_t *bst_search_with_cmp(bst_node_t *root, char key, int *comparisons);
bst_node_t *bst_delete(bst_node_t *root, char key);
bst_node_t *bst_delete_duplicates(bst_node_t *root);

// Вспомогательные
int bst_height(const bst_node_t *root);
void bst_free(bst_node_t *root);
size_t bst_memory_usage(const bst_node_t *root);

// Вывод
void in_order(bst_node_t *node, int depth);
void pre_order(bst_node_t *node, int depth);
void post_order(bst_node_t *node, int depth);
void generate_dot_highlight(bst_node_t *root, FILE *file, int highlight_duplicates);
int bst_create_dot(bst_node_t *root, const char *filename, int highlight_duplicates);

char *extract_keys_with_duplicates(const bst_node_t *root, int *total_count);

#endif