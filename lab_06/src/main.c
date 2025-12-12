#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "errors.h"
#include "define.h"
#include "print.h"
#include "node.h"
#include "output.h"
#include "compare.h"

void clear_input_buffer(void);
int is_string(char *string);

int main(void)
{
    node_t *root = NULL, *found = NULL;
    int choice, rc = ERROR_OK, temp_rc;
    char key, str[MAX_STRING_SIZE];

    printf("Программа работы с деревом.\n");
    
    do {
        // Вывод меню
        print_menu();
        if (scanf("%d", &choice) != 1)
        {
            rc = ERROR_IO;
            break;
        }
        
        switch (choice)
        {
            // Добавить строку в дерево
            case 1:
            {
                printf("Введите строку: ");
                scanf("%s", str);
                if (is_string(str))
                {
                    for (int i = 0; str[i] != '\0'; i++)
                    { 
                        root = insert(root, str[i]);
                        if (root == NULL)
                            rc = ERROR_MEM;
                    }
                }
                else 
                    rc = ERROR_IO;
                
                break;
            }
            // Удалить элемент из дерева
            case 2:
            {
                if (root == NULL)
                    rc = ERROR_EMPTY;
                else
                {
                    printf("Введите элемент для исключения: ");
                    clear_input_buffer();
                    scanf("%c", &key);
                    root = delete_node(root, key);
                    printf("Элемент '%c' удален.\n", key);
                }
                break;
            }
            // Вывести дерево
            case 3:
            {
                if (root == NULL)
                    printf("Дерево пустое.\n");
                else
                {
                    temp_rc = create_dot_file(root, "tree_original.dot", 0);
                    if (temp_rc == ERROR_OK)
                    {
                        system("dot -Tpng tree_original.dot -o tree_original.png");
                        printf("Двоичное дерево сохранено в tree_original.png\n");
                        show_image("tree_original.png");
                    }
                }
                break;
            }
            // Искать элемент в дереве
            case 4:
            {
                if (root == NULL)
                    printf("Дерево пустое.\n");
                else
                {
                    printf("Введите элемент для поиска: ");
                    clear_input_buffer();
                    scanf("%c", &key);

                    found = search(root, key);
                    if (found) 
                        printf("Элемент '%c' найден в дереве.\n", key);
                    else 
                        printf("Элемент '%c' не найден.\n", key);
                }
                break;
            }
            // Вывести повторяющиеся элементы
            case 5:
            {
                if (root == NULL)
                    printf("Дерево пустое.\n");
                else
                {
                    create_dot_file(root, "tree_highlight.dot", 1);
                    system("dot -Tpng tree_highlight.dot -o tree_highlight.png");
                    printf("Дерево с подсветкой сохранено в tree_highlight.png\n");
                    show_image("tree_highlight.png");
                }
                break;
            }
            // Удалить повторяющиеся элементы
            case 6:
            {
                if (root == NULL)
                    printf("Дерево пустое.\n");
                else
                {
                    root = delete_duplicates(root);
                    if (root) 
                    {
                        create_dot_file(root, "tree_no_duplicates.dot", 0);
                        system("dot -Tpng tree_no_duplicates.dot -o tree_no_duplicates.png");
                        printf("Дерево без повторяющихся элементов сохранено в tree_no_duplicates.png\n");
                        show_image("tree_no_duplicates.png");
                    } 
                    else 
                        printf("После удаления повторяющихся элементов дерево пусто!\n");
                }
                break;
            }
            // Очистить дерево
            case 7: 
            {
                free_tree(root);
                root = NULL;
                printf("Дерево очищено.\n");
                break;
            } 
            // Вывести три вида обхода дерева
            case 8:
            {
                if (root == NULL)
                    printf("Дерево пустое.\n");
                else
                {
                    printf("Инфиксный:\n");
                    in_order(root, 0);
                    printf("Префиксный:\n");
                    pre_order(root, 0);
                    printf("Постфиксный:\n");
                    post_order(root, 0);
                }
                break;
            }
            // Сравнение производительности
            case 9:
            {
                compare_delete();
                printf("\n");
                compare_find();
                printf("\n");
                compare_memory();
                break;
            }
            // Выход
            case 0:
            {
                printf("Выход из программы.\n");
                break;
            }
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
        }
        
        // Очистка буфера ввода после каждой операции
        clear_input_buffer();
        
    } while (rc == ERROR_OK && choice != 0);
    
    if (root)
    {
        free_tree(root);
        root = NULL;
    }

    if (rc != ERROR_OK)
        print_error(rc);

    return rc;
}

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int is_string(char *string)
{
    for (size_t i = 0; string[i] != '\0'; i++)
        if (!((string[i] >= 'A' && string[i] <= 'Z') || (string[i] >= 'a' && string[i] <= 'z')))
            return 0;

    return 1;
}