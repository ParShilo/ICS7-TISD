#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "node.h"

node_t *create_node(char data)
{
    node_t *node = malloc(sizeof(node_t));
    if (node == NULL)
        return NULL;

    node->data = data;
    node->count = 1;
    node->left = node->right = NULL;
    return node;
}

node_t *insert(node_t *root, char data)
{
    if (root == NULL) 
        return create_node(data);

    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);
    else
        root->count++;

    return root;
}

node_t *search(node_t *root, char key) 
{
    if (!root || root->data == key)
        return root;
    if (key < root->data)
        return search(root->left, key);

    return search(root->right, key);
}

void in_order(node_t *node, int depth) 
{
    if (node == NULL) 
        return;

    in_order(node->left, depth + 1); 
    printf("%c (%d) \n", node->data, node->count);
    in_order(node->right, depth + 1); 
}

void pre_order(node_t *node, int depth) 
{
    if (node == NULL) 
        return;
    
    printf("%c (%d) \n", node->data, node->count); 
    pre_order(node->left, depth + 1); 
    pre_order(node->right, depth + 1); 
}

void post_order(node_t *node, int depth) 
{
    if (node == NULL) 
        return;

    post_order(node->left, depth + 1); 
    post_order(node->right, depth + 1); 
    printf("%c (%d) \n", node->data, node->count);
}

node_t *find_min(node_t *root)
{
    while (root && root->left) 
        root = root->left;

    return root;
}

node_t *delete_node(node_t *root, char key)
{
    if (root == NULL) 
        return NULL; 

    if (key < root->data) 
        root->left = delete_node(root->left, key);
    else if (key > root->data) 
        root->right = delete_node(root->right, key);
    else 
    {
        if (root->left == NULL) 
        {
            node_t *temp = root->right;
            free(root);
            return temp;
        } 
        else if (root->right == NULL) 
        {
            node_t *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            node_t *temp = find_min(root->right);
            root->data = temp->data;
            root->right = delete_node(root->right, temp->data);
        }
    }

    return root;
}

node_t *delete_duplicates(node_t *root)
{
    if (root == NULL) 
        return NULL;

    root->left = delete_duplicates(root->left);
    root->right = delete_duplicates(root->right);

    if (root->count > 1) 
    {
        if (root->left == NULL) 
        {
            node_t *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) 
        {
            node_t *temp = root->left;
            free(root);
            return temp;
        }
        else 
        {
            node_t *temp = find_min(root->right);
            root->data = temp->data;
            root->count = 1;  
            root->right = delete_node(root->right, temp->data);
        }
    }

    return root;
}

void free_tree(node_t *root)
{
    if (root == NULL) 
        return;

    free_tree(root->left);
    free_tree(root->right);

    free(root); 
}

int tree_height(const node_t *root)
{
    if (root == NULL)
        return 0;
    
    int left_height = tree_height(root->left);
    int right_height = tree_height(root->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}

size_t tree_memory_usage(const node_t *root)
{
    if (root == NULL)
        return 0;
    
    return sizeof(node_t) + tree_memory_usage(root->left) + tree_memory_usage(root->right);
}