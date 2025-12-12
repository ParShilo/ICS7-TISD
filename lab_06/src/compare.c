#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "define.h"
#include "node.h"
#include "compare.h"

void delete_str_duplicates(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) 
    {
        for (size_t j = i + 1; j < len; j++) 
        {
            if (str[i] == str[j]) 
            {
                for (size_t k = j; k < len; k++) 
                    str[k] = str[k + 1];

                len--;
                j--;
            }
        }
    }
}

int search_in_str(const char *str, char key) 
{
    for (size_t i = 0; str[i] != '\0'; i++) 
        if (str[i] == key) 
            return 1;
    return 0;
}

void create_ordered_str(char *str, int size)
{
    for (int i = 0; i < size - 1; i++) 
        str[i] = 'a' + i % 26;  

    str[size - 1] = '\0';
}

void create_random_str(char *str, int size)
{
    for (int i = 0; i < size - 1; i++) 
        str[i] = 'a' + rand() % 26;  

    str[size - 1] = '\0';
}

node_t *create_tree_from_string(const char *str)
{
    node_t *root = NULL;
    for (int i = 0; str[i]; i++)
        root = insert(root, str[i]);

    return root;
}

void compare_delete(void)
{
    int sizes[] = {100, 200, 500, 1000, 2000, 5000, 10000}, runs = 200; 
    int h_random, h_ordered;

    char ordered_str[MAX_STRING_SIZE];
    char random_str[MAX_STRING_SIZE];

    double time_deleting_str;
    double time_deleting_tree_random;
    double time_deleting_tree_ordered;

    node_t *root_random = NULL;
    node_t *root_ordered = NULL;
    node_t *temp_random = NULL;
    node_t *temp_ordered = NULL;

    clock_t start, end;

    printf("                                          >>> СРАВНЕНИЕ АЛГОРИТМА УДАЛЕНИЯ ДУБЛИКАТОВ <<<\n");
    printf("+-----------------+-----------------+-------------------+-------------------------------+---------------------------------+-----------------------------+\n");
    printf("| Кол-во символов | Высота случ. д. | Высота вырожд. д. | Время удаления случ. д., мкс. | Время удаления вырожд. д., мкс. | Время удаления строки, мкс. |\n");
    printf("+-----------------+-----------------+-------------------+-------------------------------+---------------------------------+-----------------------------+\n");

    for (int i = 0; i < (int)(sizeof(sizes) / sizeof(sizes[0])); i++) 
    {
        create_ordered_str(ordered_str, sizes[i]);

        // Удаление из рандомной строки
        time_deleting_str = 0;
        for (int j = 0; j < runs; j++)
        {
            create_random_str(random_str, sizes[i]);
            start = clock();
            delete_str_duplicates(random_str);
            end = clock();
            time_deleting_str += (double)(end - start) / CLOCKS_PER_SEC * 1e6;
        }
        time_deleting_str /= (double)runs;

        // Удаление из балансированного дерева
        time_deleting_tree_random = 0;
        for (int j = 0; j < runs; j++)
        {
            create_random_str(random_str, sizes[i]);
            root_random = create_tree_from_string(random_str);
            start = clock();
            root_random = delete_duplicates(root_random);
            end = clock();
            free_tree(root_random);
            root_random = NULL;
            time_deleting_tree_random += (double)(end - start) / CLOCKS_PER_SEC * 1e6;
        }
        time_deleting_tree_random /= (double)runs;

        // Удаление из небалансированного дерева
        time_deleting_tree_ordered = 0;
        for (int j = 0; j < runs; j++)
        {
            root_ordered = create_tree_from_string(ordered_str);
            start = clock();
            root_ordered = delete_duplicates(root_ordered);
            end = clock();
            free_tree(root_ordered);
            root_ordered = NULL;
            time_deleting_tree_ordered += (double)(end - start) / CLOCKS_PER_SEC * 1e6;
        }
        time_deleting_tree_ordered /= (double)runs;
        
        // Нахождение высоты дерева
        temp_random = create_tree_from_string(random_str);
        h_random = tree_height(temp_random);

        temp_ordered = create_tree_from_string(ordered_str);
        h_ordered = tree_height(temp_ordered);

        free_tree(temp_random);
        free_tree(temp_ordered);

        printf("| %-15d | %-15d | %-17d | %-29.3f | %-31.3f | %-27.3f |\n", sizes[i], h_random, h_ordered, time_deleting_tree_random, time_deleting_tree_ordered, time_deleting_str);
        printf("+-----------------+-----------------+-------------------+-------------------------------+---------------------------------+-----------------------------+\n");
    }
}

void compare_memory(void)
{
    int sizes[] = {100, 200, 500, 1000, 2000, 5000, 10000};
    printf("      >>> СРАВНЕНИЕ ЗАНИМАЕМОЙ ПАМЯТИ <<<\n");
    printf("+------------------+--------------+--------------+\n");
    printf("| Кол-во элементов | Строка, байт | Дерево, байт |\n");
    printf("+------------------+--------------+--------------+\n");
    for (int i = 0; i < (int)(sizeof(sizes) / sizeof(sizes[0])); i++)
    {
        printf("| %16d | %12ld | %12ld |\n", sizes[i], sizeof(char) * sizes[i], sizeof(node_t) * sizes[i]);
        printf("+------------------+--------------+--------------+\n");
    }
}

void compare_find(void) 
{
    int sizes[] = {100, 200, 500, 1000, 2000, 5000, 10000}, runs = 200; 
    int h_random, h_ordered;

    char ordered_str[MAX_STRING_SIZE];
    char random_str[MAX_STRING_SIZE];

    double time_finding_str;
    double time_finding_tree_random;
    double time_finding_tree_ordered;

    node_t *root_random = NULL;
    node_t *root_ordered = NULL;
    node_t *temp_random = NULL;
    node_t *temp_ordered = NULL;

    clock_t start, end;

    printf("                                          >>> СРАВНЕНИЕ АЛГОРИТМА ПОИСКА ЭЛЕМЕНТА <<<\n");
    printf("+-----------------+-----------------+-------------------+-------------------------------+---------------------------------+-----------------------------+\n");
    printf("| Кол-во символов | Высота случ. д. | Высота вырожд. д. | Время поиска случ. д., мкс.   | Время поиска вырожд. д., мкс.   | Время поиска строки, мкс.   |\n");
    printf("+-----------------+-----------------+-------------------+-------------------------------+---------------------------------+-----------------------------+\n");

    for (int i = 0; i < (int)(sizeof(sizes) / sizeof(sizes[0])); i++) 
    {
        create_ordered_str(ordered_str, sizes[i]);

        // Поиск в строке
        time_finding_str = 0;
        for (int j = 0; j < runs; j++)
        {
            create_random_str(random_str, sizes[i]);
            start = clock();
            search_in_str(random_str, 'z');
            end = clock();
            time_finding_str += (double)(end - start) / CLOCKS_PER_SEC * 1e6;
        }
        time_finding_str /= (double)runs;

        // Поиск в балансированном дереве
        time_finding_tree_random = 0;
        for (int j = 0; j < runs; j++)
        {
            create_random_str(random_str, sizes[i]);
            root_random = create_tree_from_string(random_str);
            start = clock();
            search(root_random, 'z');
            end = clock();
            free_tree(root_random);
            root_random = NULL;
            time_finding_tree_random += (double)(end - start) / CLOCKS_PER_SEC * 1e6;
        }
        time_finding_tree_random /= (double)runs;

        // Удаление в небалансированном дереве
        time_finding_tree_ordered = 0;
        for (int j = 0; j < runs; j++)
        {
            root_ordered = create_tree_from_string(ordered_str);
            start = clock();
            search(root_ordered, 'z');
            end = clock();
            free_tree(root_ordered);
            root_ordered = NULL;
            time_finding_tree_ordered += (double)(end - start) / CLOCKS_PER_SEC * 1e6;
        }
        time_finding_tree_ordered /= (double)runs;
        
        // Нахождение высоты дерева
        temp_random = create_tree_from_string(random_str);
        h_random = tree_height(temp_random);

        temp_ordered = create_tree_from_string(ordered_str);
        h_ordered = tree_height(temp_ordered);

        free_tree(temp_random);
        free_tree(temp_ordered);

        printf("| %-15d | %-15d | %-17d | %-29.3f | %-31.3f | %-27.3f |\n", sizes[i], h_random, h_ordered, time_finding_tree_random, time_finding_tree_ordered, time_finding_str);
        printf("+-----------------+-----------------+-------------------+-------------------------------+---------------------------------+-----------------------------+\n");
    }
}