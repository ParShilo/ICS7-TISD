#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "errors.h"
#include "../inc/define.h"
#include "../inc/print.h"
#include "../inc/bst_node.h"
#include "../inc/avl_node.h"
#include "../inc/hashtable_chain.h"
#include "../inc/hashtable_open.h"
#include "../inc/compare.h"

void clear_input_buffer(void);
int is_string(char *string);
void show_image(const char *image_path);

int main(void)
{
    // Деревья
    bst_node_t *bst_root = NULL, *bst_found = NULL;
    avl_node_t *avl_root = NULL, *avl_found = NULL;

    // Хеш-таблицы
    hashtable_chain_t *ht_chain = NULL;
    hashtable_open_t *ht_open = NULL;

    int choice, rc = ERROR_OK, temp_rc;
    char key;
    char str[MAX_STRING_SIZE];

    if (rc == ERROR_OK)
    {
        printf("Программа работы с деревьями и хеш-таблицами.\n");

        do
        {
            print_menu();
            if (scanf("%d", &choice) != 1)
            {
                rc = ERROR_IO;
                break;
            }
            clear_input_buffer();

            switch (choice)
            {
/* =============== BST =============== */
                // Добавить строку в BST
                case 1:
                {
                    printf("Введите строку: ");
                    if (fgets(str, sizeof(str), stdin) != NULL)
                    {
                        str[strcspn(str, "\n")] = '\0';

                        if (is_string(str))
                        {
                            for (size_t i = 0; str[i]; i++)
                            {
                                bst_root = bst_insert(bst_root, str[i]);
                                if (bst_root == NULL)
                                    rc = ERROR_MEM;
                            }
                        }
                        else 
                            rc = ERROR_IO;
                    }
                    else 
                        rc = ERROR_IO;
                    break;
                }
                // Удалить элемент из BST
                case 2:
                {
                    if (bst_root == NULL)
                        printf("BST пустое.\n");
                    else
                    {
                        printf("Введите элемент для исключения: ");
                        
                        scanf("%c", &key);
                        bst_root = bst_delete(bst_root, key);
                        printf("Элемент '%c' удален из дерева.\n", key);
                    }
                    break;
                }
                // Вывести BST
                case 3:
                {
                    if (bst_root == NULL)
                        printf("Дерево пустое.\n");
                    else
                    {
                        temp_rc = bst_create_dot(bst_root, "tree_original.dot", 0);
                        if (temp_rc == ERROR_OK)
                        {
                            system("dot -Tpng tree_original.dot -o tree_original.png");
                            printf("Двоичное дерево сохранено в tree_original.png\n");
                            show_image("tree_original.png");
                        }
                    }
                    break;
                }
                // Искать элемент в BST
                case 4:
                {
                    if (bst_root == NULL)
                        printf("Дерево пустое.\n");
                    else
                    {
                        printf("Введите элемент для поиска: ");
                        
                        scanf("%c", &key);

                        bst_found = bst_search(bst_root, key);
                        if (bst_found) 
                            printf("Элемент: '%c' (%d) найден в дереве.\n", key, bst_found->count);
                        else 
                            printf("Элемент '%c' не найден.\n", key);
                    }
                    break;
                }
                // Вывести повторяющиеся BST
                case 5:
                {
                    if (bst_root == NULL)
                        printf("Дерево пустое.\n");
                    else
                    {
                        bst_create_dot(bst_root, "tree_highlight.dot", 1);
                        system("dot -Tpng tree_highlight.dot -o tree_highlight.png");
                        printf("Дерево с подсветкой сохранено в tree_highlight.png\n");
                        show_image("tree_highlight.png");
                    }
                    break;
                }
                // Удалить повторяющиеся BST
                case 6:
                {
                    if (bst_root == NULL)
                        printf("Дерево пустое.\n");
                    else
                    {
                        bst_root = bst_delete_duplicates(bst_root);
                        if (bst_root) 
                        {
                            bst_create_dot(bst_root, "tree_no_duplicates.dot", 0);
                            system("dot -Tpng tree_no_duplicates.dot -o tree_no_duplicates.png");
                            printf("Дерево без повторяющихся элементов сохранено в tree_no_duplicates.png\n");
                            show_image("tree_no_duplicates.png");
                        } 
                        else 
                            printf("После удаления повторяющихся элементов дерево пусто!\n");
                    }
                    break;
                }
                // Очистить BST
                case 7: 
                {
                    bst_free(bst_root);
                    bst_root = NULL;
                    printf("Дерево очищено.\n");
                    break;
                } 
                // Сбалансировать BST
                case 8: 
                {
                    if (bst_root == NULL)
                    {
                        printf("BST пустое. Нечего сбалансировать.\n");
                        break;
                    }
                    
                    int count = 0;
                    char *keys = extract_keys_with_duplicates(bst_root, &count);
                    if (keys == NULL)
                    {
                        rc = ERROR_MEM;
                        break;
                    }

                    avl_free(avl_root);
                    avl_root = NULL;
                    for (int i = 0; rc == ERROR_OK && i < count; i++)
                    {
                        avl_root = avl_insert(avl_root, keys[i]);

                        if (avl_root == NULL)
                        {
                            free(keys);
                            rc = ERROR_MEM;
                        }
                    }

                    if (rc == ERROR_OK)
                    {
                        free(keys);
                        printf("AVL-дерево построено на основе BST.\n");
                        avl_create_dot(avl_root, "avl.dot", 0);
                        system("dot -Tpng avl.dot -o avl.png 2>nul || dot -Tpng avl.dot -o avl.png");
                        printf("AVL сохранено в avl.png\n");
                        show_image("avl.png");
                    }

                    break;
                }
/* =============== AVL =============== */
                // Добавить строку в AVL
                case 11: 
                {
                    printf("Введите строку: ");
                    
                    if (fgets(str, sizeof(str), stdin) != NULL)
                    {
                        str[strcspn(str, "\n")] = '\0';

                        if (is_string(str))
                        {
                            for (size_t i = 0; str[i]; i++)
                            {
                                avl_root = avl_insert(avl_root, str[i]);
                                if (avl_root == NULL)
                                    rc = ERROR_MEM;
                            }
                        }
                        else 
                            rc = ERROR_IO;
                    }
                    else 
                        rc = ERROR_IO;
                    break;
                }
                // Удалить из AVL
                case 12: 
                {
                    if (avl_root == NULL)
                        printf("AVL пустое.\n");
                    else
                    {
                        printf("Введите элемент для удаления: ");
                        
                        scanf("%c", &key);
                        avl_root = avl_delete(avl_root, key);
                        printf("Элемент '%c' удалён из AVL.\n", key);
                    }
                    break;
                }
                // Вывод AVL
                case 13: 
                {
                    if (avl_root == NULL)
                        printf("AVL пустое.\n");
                    else
                    {
                        avl_create_dot(avl_root, "avl.dot", 0);
                        system("dot -Tpng avl.dot -o avl.png 2>nul || dot -Tpng avl.dot -o avl.png");
                        printf("AVL сохранено в avl.png\n");
                        show_image("avl.png");
                    }
                    break;
                }
                // Поиск в AVL
                case 14: 
                {
                    if (avl_root == NULL)
                        printf("AVL пустое.\n");
                    else
                    {
                        printf("Введите элемент для поиска: ");
                        
                        scanf("%c", &key);
                        avl_found = avl_search(avl_root, key);
                        if (avl_found)
                            printf("Элемент '%c' найден в AVL.\n", key);
                        else
                            printf("Элемент '%c' не найден в AVL.\n", key);
                    }
                    break;
                }
                // Подсветить повторяющиеся (AVL)
                case 15: 
                {
                    if (avl_root == NULL)
                        printf("AVL пустое.\n");
                    else
                    {
                        avl_create_dot(avl_root, "avl_dup.dot", 1);
                        system("dot -Tpng avl_dup.dot -o avl_dup.png 2>nul || dot -Tpng avl_dup.dot -o avl_dup.png");
                        printf("AVL с подсветкой повторов сохранено в avl_dup.png\n");
                        show_image("avl_dup.png");
                    }
                    break;
                }
                // Очистка AVL
                case 16: 
                {
                    avl_free(avl_root);
                    avl_root = NULL;
                    printf("AVL очищено.\n");
                    break;
                }
/* =============== Хеш-таблица (цепочки) =============== */
                // Добавить строку в хеш (цепочки)            
                case 21: 
                {
                    printf("Введите строку: ");
                    
                    if (fgets(str, sizeof(str), stdin) != NULL)
                    {
                        str[strcspn(str, "\n")] = '\0';

                        if (is_string(str) && strlen(str) > 0)
                        {
                            if (ht_chain == NULL)
                            {
                                int seen[256] = {0};
                                int n_unique = 0;

                                for (size_t i = 0; str[i]; i++)
                                {
                                    unsigned char c = (unsigned char)str[i];
                                    if (!seen[c])
                                    {
                                        seen[c] = 1;
                                        n_unique++;
                                    }
                                }

                                int initial_size = (int)ceil(n_unique / 0.72);
                                if (initial_size < 1)
                                    initial_size = 1;

                                ht_chain = ht_chain_create(initial_size);
                                if (ht_chain == NULL)
                                {
                                    rc = ERROR_MEM;
                                    break;
                                }
                                printf("Хеш-таблица создана (размер = %d для %d уникальных букв).\n", ht_chain->size, n_unique);
                            }

                            for (size_t i = 0; str[i]; i++)
                                ht_chain_insert(ht_chain, str[i]);
                        }
                        else 
                            rc = ERROR_IO;
                    }
                    else 
                        rc = ERROR_IO;
                    break;
                }
                // Удалить из хеш (цепочки)
                case 22: 
                {
                    if (ht_chain == NULL)
                        printf("Хэш-таблица пуста.\n");
                    else
                    {
                        printf("Введите элемент для удаления: ");
                        scanf("%c", &key);
                        ht_chain_delete(ht_chain, key);
                        printf("Элемент '%c' удалён из хеш-таблицы (цепочки).\n", key);
                    }
                    break;
                }
                // Поиск в хеш (цепочки)
                case 23: 
                {
                    if (ht_chain == NULL)
                        printf("Хэш-таблица пуста.\n");
                    else
                    {
                        printf("Введите элемент для поиска: ");
                        scanf("%c", &key);
                        int comparisons = 0;
                        int cnt = ht_chain_search(ht_chain, key, &comparisons);
                        if (cnt > 0)
                            printf("Элемент '%c' найден (%d раз) в хеш-таблице (цепочки) за %d кол-во сравнений.\n", key, cnt, comparisons);
                        else
                            printf("Элемент '%c' не найден.\n", key);

                        if (comparisons > 4)
                        {
                            printf("Так как кол-во сравнений больше 4, выполняется реструктуризация:\n");
                            printf("Текущий размер: %d, элементов: %d\n", ht_chain->size, ht_chain->num_elements);
                            ht_chain = ht_chain_rehash(ht_chain);
                            if (ht_chain == NULL)
                                rc = ERROR_MEM;
                            else
                                printf("Реструктуризация выполнена. Новый размер: %d\n", ht_chain->size);
                        }
                    }
                    break;
                }
                // Вывод хеш (цепочки)
                case 24: 
                {
                    if (ht_chain == NULL)
                        printf("Хэш-таблица пуста.\n");
                    else
                        ht_chain_print(ht_chain);
                    break;
                }
                // Реструктуризация (цепочки)
                case 25: 
                {


                    if (ht_chain == NULL)
                        printf("Хэш-таблица пуста.\n");
                    else
                    {
                        double load_factor = (double)ht_chain->num_elements / ht_chain->size;
                        double avg_cmp = ht_chain_avg_comparisons(ht_chain);

                        printf("Текущий размер: %d, элементов: %d, процент загруженности: %.2f, кол-во сравнений: %.2f.\n", ht_chain->size, ht_chain->num_elements, load_factor, avg_cmp);

                        if (load_factor <= 0.72 && avg_cmp <= 4.0)
                            printf("Реструктуризация не требуется!\n");
                        else
                        {
                            ht_chain = ht_chain_rehash(ht_chain);
                            if (ht_chain == NULL)
                                rc = ERROR_MEM;
                            else
                                printf("Реструктуризация выполнена. Новый размер: %d\n", ht_chain->size);
                        }
                    }
                    break;
                }
                // Очистка хеш (цепочки)
                case 26: 
                {
                    if (ht_chain == NULL)
                        printf("Хэш-таблица пуста.\n");
                    else
                    {
                        ht_chain_destroy(ht_chain);
                        ht_chain = NULL;
                        printf("Хеш-таблица (цепочки) очищена.\n");
                    }
                    break;
                }
/* =============== Хеш-таблица (линейная адресация) =============== */
                // Добавить строку (адресация)
                case 31:
                {
                    printf("Введите строку: ");
                    
                    if (fgets(str, sizeof(str), stdin) != NULL)
                    {
                        str[strcspn(str, "\n")] = '\0';

                        if (is_string(str) && strlen(str) > 0)
                        {
                            if (ht_open == NULL)
                            {
                                int seen[256] = {0};
                                int n_unique = 0;

                                for (size_t i = 0; str[i]; i++)
                                {
                                    unsigned char c = (unsigned char)str[i];
                                    if (!seen[c])
                                    {
                                        seen[c] = 1;
                                        n_unique++;
                                    }
                                }

                                int initial_size = (int)ceil(n_unique * 1.2);
                                if (initial_size < 1)
                                    initial_size = 1;

                                ht_open = ht_open_create(initial_size);
                                if (ht_open == NULL)
                                {
                                    rc = ERROR_MEM;
                                    break;
                                }
                                printf("Хеш-таблица создана (размер = %d для %d уникальных букв).\n", ht_open->size, n_unique);
                            }

                            for (size_t i = 0; str[i]; i++)
                            if (ht_open_insert(ht_open, str[i]) == 0)
                            {
                                printf("Ошибка: таблица заполнена!\n");
                                break;
                            }
                        }
                        else 
                            rc = ERROR_IO;
                    }
                    else 
                        rc = ERROR_IO;
                    break;
                }
                // Удалить (адресация)
                case 32:
                {
                    if (ht_open == NULL)
                    {
                        printf("Хеш-таблица (адресация) пуста.\n");
                        break;
                    }
                    printf("Введите элемент для удаления: ");
                    scanf("%c", &key);
                    if (ht_open_delete(ht_open, key))
                        printf("Элемент '%c' удалён.\n", key);
                    else
                        printf("Элемент '%c' не найден.\n", key);
                    break;
                }
                // Поиск (адресация)
                case 33:
                {
                    if (ht_open == NULL)
                    {
                        printf("Хеш-таблица (адресация) пуста.\n");
                        break;
                    }
                    printf("Введите элемент для поиска: ");
                    scanf("%c", &key);
                    int cmp = 0;
                    int cnt = ht_open_search(ht_open, key, &cmp);
                    if (cnt > 0)
                        printf("Элемент '%c' найден (%d раз) за %d сравнений.\n", key, cnt, cmp);
                    else
                        printf("Элемент '%c' не найден (сравнений: %d).\n", key, cmp);

                    if (cmp > 4)
                    {
                        printf("Так как кол-во сравнений больше 4, выполняется реструктуризация:\n");
                        ht_open = ht_open_rehash(ht_open);
                        if (ht_open == NULL)
                            rc = ERROR_MEM;
                        else
                            printf("Новый размер: %d\n", ht_open->size);
                    }
                    break;
                }
                // Вывод (адресация)
                case 34:
                {
                    if (ht_open == NULL)
                        printf("Хеш-таблица (адресация) пуста.\n");
                    else
                        ht_open_print(ht_open);
                    break;
                }
                // Реструктуризация (адресация)
                case 35:
                {
                    if (ht_open == NULL)
                        printf("Хеш-таблица (адресация) пуста.\n");
                    else
                    {
                        double load_factor = (double)ht_open->num_elements / ht_open->size;
                        double avg_cmp = ht_open_avg_comparisons(ht_open);

                        printf("Текущий размер: %d, элементов: %d, процент загруженности: %.2f, кол-во сравнений: %.2f.\n",
                            ht_open->size, ht_open->num_elements, load_factor, avg_cmp);

                        if (load_factor <= 0.7 && avg_cmp <= 4.0)
                            printf("Реструктуризация не требуется!\n");
                        else
                        {
                            ht_open = ht_open_rehash(ht_open);
                            if (ht_open == NULL)
                                rc = ERROR_MEM;
                            else
                                printf("Реструктуризация выполнена. Новый размер: %d\n", ht_open->size);
                        }
                    }
                    break;
                }
                // Очистка (адресация)
                case 36:
                {
                    if (ht_open)
                    {
                        ht_open_destroy(ht_open);
                        ht_open = NULL;
                        printf("Хеш-таблица (адресация) очищена.\n");
                    }
                    else
                        printf("Хеш-таблица уже пуста.\n");
                    break;
                }
/* =============== Замеры =============== */
                case 100:
                {
                    printf("Запуск замеров производительности...\n");
                    compare_all_structures();
                    break;
                } 

// =============== Выход ===============
                case 0:
                {
                    printf("Выход из программы.\n");
                    break;
                }

                default:
                    printf("Неверный выбор! Попробуйте снова.\n");
                    break;
            }
        } while (rc == ERROR_OK && choice != 0);
    }

    // Освобождение памяти
    bst_free(bst_root);
    avl_free(avl_root);
    ht_chain_destroy(ht_chain);
    ht_open_destroy(ht_open);

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
    if (string == NULL || strlen(string) == 0)
        return 0;
    for (size_t i = 0; string[i] != '\0'; i++)
    {
        if (!((string[i] >= 'A' && string[i] <= 'Z') ||
              (string[i] >= 'a' && string[i] <= 'z')))
            return 0;
    }
    return 1;
}

void show_image(const char *image_path)
{
    char command[256];
    snprintf(command, sizeof(command), "open %s", image_path);
    system(command);
}