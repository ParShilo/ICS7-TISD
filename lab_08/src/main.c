#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "errors.h"
#include "../inc/define.h"
#include "../inc/print.h"
#include "../inc/graph_node.h"

void clear_input_buffer(void);
void show_image(const char *image_path);

int main(void)
{
    graph_t *graph = NULL;
    int choice, rc = ERROR_OK;

    printf("Программа анализа связности островов.\n");

    do
    {
        print_menu();

        if (scanf("%d", &choice) != 1)
        {
            rc = ERROR_IO;
            clear_input_buffer();
            break;
        }
        clear_input_buffer();

        switch (choice)
        {
            // Ввести острова и пути между ними
            case 1:
            {
                if (graph != NULL)
                {
                    graph_free(graph);
                    graph = NULL;
                }

                printf("Введите начальное количество островов: ");
                int n;
                if (scanf("%d", &n) != 1 || n <= 0)
                {
                    printf("Неверное количество островов.\n");
                    break;
                }

                graph = graph_create(n);
                if (graph == NULL)
                {
                    printf("Ошибка создания графа.\n");
                    rc = ERROR_MEM;
                    break;
                }

                printf("Введите количество путей: ");
                int m;
                if (scanf("%d", &m) != 1 || m < 0)
                {
                    printf("Неверное количество путей.\n");
                    break;
                }

                int total = m;
                for (int i = 0; i < m; i++)
                {
                    printf("Путь %d: введите два острова (от 1 до %d): ", i + 1, n);
                    int u, v;
                    if (scanf("%d %d", &u, &v) != 2)
                    {
                        printf("Ошибка ввода пути.\n");
                        break;
                    }

                    if (u < 1 || u > n || v < 1 || v > n)
                    {
                        printf("Остров вне диапазона [1, %d]. Путь пропущен.\n", n);
                        total--;
                        continue;
                    }

                    if (graph_add_edge(graph, u - 1, v - 1, 0) == 0)
                        total--;
                }

                printf("Карта из %d островов и %d путей создана.\n", n, total);
                break;
            }
             // Добавить остров
            case 2:
            {
                rc = graph_add_vertex(&graph);
                if (rc == ERROR_OK)
                    printf("Добавлен новый остров. Всего: %d\n", graph->num_vertices);
                else
                {
                    printf("Не удалось добавить остров.\n");
                    rc = ERROR_MEM;
                }
                break;
            }
            // Добавить путь между существующими островами
            case 3:
            {
                if (graph == NULL || graph->num_vertices == 0)
                {
                    printf("Нет островов! Сначала добавьте остров.\n");
                    break;
                }

                printf("Введите два острова (от 1 до %d): ", graph->num_vertices);
                int u, v;
                if (scanf("%d %d", &u, &v) != 2)
                {
                    printf("Неверный формат.\n");
                    clear_input_buffer();
                    break;
                }

                if (u < 1 || u > graph->num_vertices || v < 1 || v > graph->num_vertices)
                {
                    printf("Остров вне допустимого диапазона.\n");
                    break;
                }

                graph_add_edge(graph, u - 1, v - 1, 0);
                break;
            }
            // Проверить связность
            case 4: 
            {
                if (graph == NULL || graph->num_vertices == 0)
                {
                    printf("Нет островов.\n");
                    break;
                }

                int connected = graph_is_connected(graph);
                if (connected)
                    printf("Все острова связаны.\n");
                else
                    printf("Острова разъединены.\n");
                break;
            }
            // Вывести все пути (графически)
            case 5: 
            {
                if (graph == NULL || graph->num_vertices == 0)
                {
                    printf("Нет островов для визуализации.\n");
                    break;
                }

                rc = graph_create_dot(graph, "graph.dot");
                if (rc == ERROR_OK)
                {
                    system("dot -Tpng graph.dot -o graph.png 2>nul || dot -Tpng graph.dot -o graph.png");
                    printf("Граф сохранён в graph.png\n");
                    show_image("graph.png");
                }
                else
                    printf("Ошибка при создании изображения.\n");
                break;
            }
            // Замер статистики
            case 6:
            {
                if (statistic() != ERROR_OK)
                    rc = ERROR_MEM;
                break;
            }
            // Очистить всё
            case 7: 
            {
                if (graph != NULL)
                {
                    graph_free(graph);
                    graph = NULL;
                    printf("Карта очищена.\n");
                }
                else
                    printf("Карта и так пуста.\n");
                break;
            }

            case 0:
                printf("Выход.\n");
                break;

            default:
                printf("Неверный выбор.\n");
                break;
        }

    } while (rc == ERROR_OK && choice != 0);

    if (graph != NULL)
        graph_free(graph);

    if (rc != ERROR_OK)
        print_error(rc);

    return rc;
}

void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void show_image(const char *image_path)
{
    char command[256];
    snprintf(command, sizeof(command), "open %s", image_path);
    system(command);
}