#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "errors.h"
#include "output.h"

void generate_dot_highlight(node_t *root, FILE *file, int highlight_duplicates)
{
    if (root == NULL) 
        return;

    if (highlight_duplicates && root->count > 1) 
        fprintf(file, "    \"%c, (%d)\" [style=filled, fillcolor=red, fontcolor=black];\n", root->data, root->count);
    else 
        fprintf(file, "    \"%c, (%d)\" [fontcolor=white, color=white];\n", root->data, root->count);

    if (root->left) 
    {
        fprintf(file, "    \"%c, (%d)\" -> \"%c, (%d)\" [label=\"Л\", fontcolor=white, color=white];\n",
                root->data, root->count,
                root->left->data, root->left->count);
        generate_dot_highlight(root->left, file, highlight_duplicates);
    }

    if (root->right) 
    {
        fprintf(file, "    \"%c, (%d)\" -> \"%c, (%d)\" [label=\"П\", fontcolor=white, color=white];\n",
                root->data, root->count,
                root->right->data, root->right->count);
        generate_dot_highlight(root->right, file, highlight_duplicates);
    }
}

int create_dot_file(node_t *root, const char *filename, int highlight_duplicates)
{
    if (root == NULL) 
    {
        printf("Дерево пустое.\n");
        return ERROR_IO;  
    }

    FILE *file = fopen(filename, "w");
    if (!file) 
    {
        perror("Ошибка при создании DOT-файла");
        return ERROR_IO;
    }

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

void show_image(const char *image_path)
{
    char command[256];
    snprintf(command, sizeof(command), "open %s", image_path);
    system(command);
}