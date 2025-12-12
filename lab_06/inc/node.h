#ifndef NODE_H__
#define NODE_H__

#include "define.h"

node_t *create_node(char data);
node_t *insert(node_t *root, char data);
node_t *search(node_t *root, char key);
void in_order(node_t *node, int depth);
void pre_order(node_t *node, int depth);
void post_order(node_t *node, int depth);
node_t *find_min(node_t *root);
node_t *delete_node(node_t *root, char key);
node_t *delete_duplicates(node_t *root);
void free_tree(node_t *root);
int tree_height(const node_t *root);
size_t tree_memory_usage(const node_t *root);

#endif