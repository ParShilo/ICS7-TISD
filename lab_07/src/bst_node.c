#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "../inc/define.h"
#include "../inc/bst_node.h"

bst_node_t *bst_create_node(char data)
{
    bst_node_t *node = malloc(sizeof(bst_node_t));
    if (node == NULL)
        return NULL;

    node->data = data;
    node->count = 1;
    node->left = node->right = NULL;
    return node;
}

bst_node_t *bst_insert(bst_node_t *root, char data)
{
    if (root == NULL) 
        return bst_create_node(data);

    if (data < root->data)
        root->left = bst_insert(root->left, data);
    else if (data > root->data)
        root->right = bst_insert(root->right, data);
    else
        root->count++;

    return root;
}

bst_node_t *bst_search(bst_node_t *root, char key) 
{
    if (!root || root->data == key)
        return root;
    if (key < root->data)
        return  bst_search(root->left, key);

    return  bst_search(root->right, key);
}

bst_node_t *bst_search_with_cmp(bst_node_t *root, char key, int *comparisons)
{
    if (!root) 
    {
        if (comparisons) *comparisons = 0;
        return NULL;
    }
    (*comparisons)++;
    if (root->data == key)
        return root;
    if (key < root->data)
        return bst_search_with_cmp(root->left, key, comparisons);
    return bst_search_with_cmp(root->right, key, comparisons);
}

void in_order(bst_node_t *node, int depth) 
{
    if (node == NULL) 
        return;

    in_order(node->left, depth + 1); 
    printf("%c (%d) \n", node->data, node->count);
    in_order(node->right, depth + 1); 
}

void pre_order(bst_node_t *node, int depth) 
{
    if (node == NULL) 
        return;
    
    printf("%c (%d) \n", node->data, node->count); 
    pre_order(node->left, depth + 1); 
    pre_order(node->right, depth + 1); 
}

void post_order(bst_node_t *node, int depth) 
{
    if (node == NULL) 
        return;

    post_order(node->left, depth + 1); 
    post_order(node->right, depth + 1); 
    printf("%c (%d) \n", node->data, node->count);
}

bst_node_t *bst_delete(bst_node_t *root, char key)
{
    if (root == NULL) 
        return NULL; 

    if (key < root->data) 
        root->left = bst_delete(root->left, key);
    else if (key > root->data) 
        root->right = bst_delete(root->right, key);
    else 
    {
        if (root->left == NULL) 
        {
            bst_node_t *temp = root->right;
            free(root);
            return temp;
        } 
        else if (root->right == NULL) 
        {
            bst_node_t *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            bst_node_t *temp = root->right;
            while (temp && temp->left) 
                temp = temp->left;

            root->data = temp->data;
            root->right = bst_delete(root->right, temp->data);
        }
    }

    return root;
}

bst_node_t *bst_delete_duplicates(bst_node_t *root)
{
    if (root == NULL) 
        return NULL;

    root->left = bst_delete_duplicates(root->left);
    root->right = bst_delete_duplicates(root->right);

    if (root->count > 1) 
    {
        if (root->left == NULL) 
        {
            bst_node_t *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) 
        {
            bst_node_t *temp = root->left;
            free(root);
            return temp;
        }
        else 
        {
            bst_node_t *temp = root->right;
            while (temp && temp->left) 
                temp = temp->left;

            root->data = temp->data;
            root->count = 1;  
            root->right = bst_delete(root->right, temp->data);
        }
    }

    return root;
}

void bst_free(bst_node_t *root)
{
    if (root == NULL) 
        return;

    bst_free(root->left);
    bst_free(root->right);

    free(root); 
}

int bst_height(const bst_node_t *root)
{
    if (root == NULL)
        return 0;
    
    int left_height = bst_height(root->left);
    int right_height = bst_height(root->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}

size_t bst_memory_usage(const bst_node_t *root)
{
    if (root == NULL)
        return 0;
    
    return sizeof(bst_node_t) + bst_memory_usage(root->left) + bst_memory_usage(root->right);
}

static int count_total_nodes_with_duplicates(const bst_node_t *node)
{
    if (node == NULL)
        return 0;
    return node->count + 
           count_total_nodes_with_duplicates(node->left) + 
           count_total_nodes_with_duplicates(node->right);
}

static void fill_keys_array(const bst_node_t *node, char **keys_ptr)
{
    if (node == NULL)
        return;
    
    // Сначала левое поддерево
    fill_keys_array(node->left, keys_ptr);
    
    // Затем текущий узел — count раз
    for (int i = 0; i < node->count; i++)
    {
        *(*keys_ptr) = node->data;
        (*keys_ptr)++;
    }
    
    // Потом правое поддерево
    fill_keys_array(node->right, keys_ptr);
}

char *extract_keys_with_duplicates(const bst_node_t *root, int *total_count)
{
    if (!root || !total_count)
    {
        if (total_count)
            *total_count = 0;
        return NULL;
    }

    *total_count = count_total_nodes_with_duplicates(root);
    if (*total_count == 0) 
        return NULL;

    char *keys = malloc(*total_count * sizeof(char));
    if (!keys)
        return NULL;

    char *temp = keys;
    fill_keys_array(root, &temp);

    return keys;
}

void generate_dot_highlight(bst_node_t *root, FILE *file, int highlight_duplicates)
{
    if (root == NULL) 
        return;

    if (highlight_duplicates && root->count > 1) 
        fprintf(file, "    \"%c, (%d)\" [style=filled, fillcolor=red, fontcolor=black];\n", root->data, root->count);
    else 
        fprintf(file, "    \"%c, (%d)\" [fontcolor=white, color=white];\n", root->data, root->count);

    if (root->left) 
    {
        fprintf(file, "    \"%c, (%d)\" -> \"%c, (%d)\" [label=\"L\", fontcolor=white, color=white];\n",
                root->data, root->count,
                root->left->data, root->left->count);
        generate_dot_highlight(root->left, file, highlight_duplicates);
    }

    if (root->right) 
    {
        fprintf(file, "    \"%c, (%d)\" -> \"%c, (%d)\" [label=\"R\", fontcolor=white, color=white];\n",
                root->data, root->count,
                root->right->data, root->right->count);
        generate_dot_highlight(root->right, file, highlight_duplicates);
    }
}

int bst_create_dot(bst_node_t *root, const char *filename, int highlight_duplicates)
{
    if (root == NULL) 
    {
        printf("Дерево пустое.\n");
        return ERROR_IO;  
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL)
        return ERROR_IO;

    fprintf(file, "digraph BST {\n");
    fprintf(file, "    bgcolor=black;\n");
    fprintf(file, "    node [shape=circle, fontname=\"Arial\", fontcolor=white, color=white];\n");
    fprintf(file, "    edge [fontname=\"Arial\", fontcolor=white, color=white, fontsize=10];\n");

    generate_dot_highlight(root, file, highlight_duplicates);

    fprintf(file, "}\n");
    fclose(file);

    printf("DOT-файл успешно создан.\n");
    return ERROR_OK;
}