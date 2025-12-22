// avl_node.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "../inc/define.h"
#include "../inc/avl_node.h"

int avl_height(const avl_node_t *node)
{
    return node ? node->height : 0;
}

int avl_get_balance(const avl_node_t *node)
{
    if (node == NULL)
        return 0;

    return avl_height(node->left) - avl_height(node->right);
}

avl_node_t *avl_create_node(char data)
{
    avl_node_t *node = malloc(sizeof(avl_node_t));
    if (node == NULL)
        return NULL;

    node->data = data;
    node->count = 1;
    node->height = 1;
    node->left = node->right = NULL;

    return node;
}

avl_node_t *avl_rotate_right(avl_node_t *y)
{
    avl_node_t *x = y->left;
    avl_node_t *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (avl_height(y->left) > avl_height(y->right) ? avl_height(y->left) : avl_height(y->right));
    x->height = 1 + (avl_height(x->left) > avl_height(x->right) ? avl_height(x->left) : avl_height(x->right));

    return x;
}

avl_node_t *avl_rotate_left(avl_node_t *x)
{
    avl_node_t *y = x->right;
    avl_node_t *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (avl_height(x->left) > avl_height(x->right) ? avl_height(x->left) : avl_height(x->right));
    y->height = 1 + (avl_height(y->left) > avl_height(y->right) ? avl_height(y->left) : avl_height(y->right));

    return y;
}

avl_node_t *avl_insert(avl_node_t *root, char data)
{
    // 1. Обычная вставка как в BST
    if (root == NULL)
        return avl_create_node(data);

    if (data < root->data)
        root->left = avl_insert(root->left, data);
    else if (data > root->data)
        root->right = avl_insert(root->right, data);
    else
    {
        root->count++;
        return root;
    }

    // 2. Обновление высоты
    root->height = 1 + (avl_height(root->left) > avl_height(root->right) ? avl_height(root->left) : avl_height(root->right));

    // 3. Получение баланса
    int balance = avl_get_balance(root);

    // 4. Балансировка (4 случая)

    // Левый-Левый
    if (balance > 1 && data < root->left->data)
        return avl_rotate_right(root);

    // Правый-Правый
    if (balance < -1 && data > root->right->data)
        return avl_rotate_left(root);

    // Левый-Правый
    if (balance > 1 && data > root->left->data)
    {
        root->left = avl_rotate_left(root->left);
        return avl_rotate_right(root);
    }

    // Правый-Левый
    if (balance < -1 && data < root->right->data)
    {
        root->right = avl_rotate_right(root->right);
        return avl_rotate_left(root);
    }

    return root;
}

avl_node_t *avl_delete(avl_node_t *root, char key)
{
    // 1. Стандартное удаление BST
    if (root == NULL)
        return root;

    if (key < root->data)
        root->left = avl_delete(root->left, key);
    else if (key > root->data)
        root->right = avl_delete(root->right, key);
    else
    {
        if (root->left == NULL || root->right == NULL)
        {
            avl_node_t *temp = root->left ? root->left : root->right;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            avl_node_t *temp = root->right;
            while (temp && temp->left)
                temp = temp->left;

            root->data = temp->data;
            root->count = temp->count;
            root->right = avl_delete(root->right, temp->data);
        }
    }

    if (root == NULL)
        return root;

    // 2. Обновление высоты
    root->height = 1 + (avl_height(root->left) > avl_height(root->right) ? avl_height(root->left) : avl_height(root->right));

    // 3. Балансировка
    int balance = avl_get_balance(root);

    // Левый-Левый
    if (balance > 1 && avl_get_balance(root->left) >= 0)
        return avl_rotate_right(root);

    // Левый-Правый
    if (balance > 1 && avl_get_balance(root->left) < 0) {
        root->left = avl_rotate_left(root->left);
        return avl_rotate_right(root);
    }

    // Правый-Правый
    if (balance < -1 && avl_get_balance(root->right) <= 0)
        return avl_rotate_left(root);

    // Правый-Левый
    if (balance < -1 && avl_get_balance(root->right) > 0) {
        root->right = avl_rotate_right(root->right);
        return avl_rotate_left(root);
    }

    return root;
}

avl_node_t *avl_search(avl_node_t *root, char key)
{
    if (root == NULL|| root->data == key)
        return root;
    if (key < root->data)
        return avl_search(root->left, key);
    return avl_search(root->right, key);
}

avl_node_t *avl_search_with_cmp(avl_node_t *root, char key, int *comparisons)
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
        return avl_search_with_cmp(root->left, key, comparisons);
    return avl_search_with_cmp(root->right, key, comparisons);
}

void avl_free(avl_node_t *root)
{
    if (root == NULL)
        return;

    avl_free(root->left);
    avl_free(root->right);
    free(root);
}

size_t avl_memory_usage(const avl_node_t *root)
{
    if (root == NULL)
        return 0;

    return sizeof(avl_node_t) + avl_memory_usage(root->left) + avl_memory_usage(root->right);
}

int avl_tree_height(const avl_node_t *root)
{
    return avl_height(root);
}

static void avl_generate_dot_highlight(avl_node_t *root, FILE *file, int highlight_duplicates)
{
    if (root == NULL) 
        return;

    // Определяем стиль узла
    if (highlight_duplicates && root->count > 1) 
    {
        fprintf(file, "    \"%c, (%d)\" [style=filled, fillcolor=red, fontcolor=black];\n", root->data, root->count);
    }
    else 
    {
        fprintf(file, "    \"%c, (%d)\" [fontcolor=white, color=white];\n", root->data, root->count);
    }

    // Обработка левого потомка
    if (root->left) 
    {
        fprintf(file, "    \"%c, (%d)\" -> \"%c, (%d)\" [label=\"L\", fontcolor=white, color=white];\n",
                root->data, root->count,
                root->left->data, root->left->count);
        avl_generate_dot_highlight(root->left, file, highlight_duplicates);
    }

    // Обработка правого потомка
    if (root->right) 
    {
        fprintf(file, "    \"%c, (%d)\" -> \"%c, (%d)\" [label=\"R\", fontcolor=white, color=white];\n",
                root->data, root->count,
                root->right->data, root->right->count);
        avl_generate_dot_highlight(root->right, file, highlight_duplicates);
    }
}

int avl_create_dot(avl_node_t *root, const char *filename, int highlight_duplicates)
{
    if (root == NULL) 
    {
        printf("AVL-дерево пустое.\n");
        return ERROR_IO;  
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL)
        return ERROR_IO;

    fprintf(file, "digraph AVL {\n");
    fprintf(file, "    bgcolor=black;\n");
    fprintf(file, "    node [shape=circle, fontname=\"Arial\", fontcolor=white, color=white];\n");
    fprintf(file, "    edge [fontname=\"Arial\", fontcolor=white, color=white, fontsize=10];\n");

    avl_generate_dot_highlight(root, file, highlight_duplicates);

    fprintf(file, "}\n");
    fclose(file);

    printf("DOT-файл AVL успешно создан.\n");
    return ERROR_OK;
}

void avl_print_to_file(avl_node_t *root, FILE *file, int depth)
{
    if (root == NULL|| !file) 
        return;
    for (int i = 0; i < depth; i++) fprintf(file, "  ");
    fprintf(file, "%c (%d)\n", root->data, root->count);
    avl_print_to_file(root->left, file, depth + 1);
    avl_print_to_file(root->right, file, depth + 1);
}