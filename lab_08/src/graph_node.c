#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "errors.h"
#include "../inc/graph_node.h"

static adj_node_t *create_adj_node(int v)
{
    adj_node_t *node = malloc(sizeof(adj_node_t));
    if (node == NULL)
        return NULL;

    node->vertex = v;
    node->next = NULL;
    return node;
}

graph_t *graph_create(int n)
{
    if (n <= 0)
        return NULL;

    graph_t *g = malloc(sizeof(graph_t));
    if (g == NULL)
        return NULL;

    g->num_vertices = n;
    g->adj_list = calloc(n, sizeof(adj_node_t *));
    if (g->adj_list == NULL)
    {
        free(g);
        return NULL;
    }

    return g;
}

int graph_add_edge(graph_t *g, int u, int v, int quiet)
{
    if (g == NULL || u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices)
        return 0;

    adj_node_t *node = g->adj_list[u];
    int exists = 0;
    while (node != NULL)
    {
        if (node->vertex == v)
        {
            exists = 1;
            break;
        }
        node = node->next;
    }

    if (exists && quiet == 0)
    {
        printf("Такой путь уже существует.\n");
        return 0;
    }

    adj_node_t *new_node = create_adj_node(v);
    if (new_node)
    {
        new_node->next = g->adj_list[u];
        g->adj_list[u] = new_node;
    }

    if (u != v)
    {
        new_node = create_adj_node(u);
        if (new_node)
        {
            new_node->next = g->adj_list[v];
            g->adj_list[v] = new_node;
        }
    }

    if (quiet == 0)
        printf("Путь между островами %d и %d добавлен.\n", u + 1, v + 1);

    return 1;
}

static void dfs_util(graph_t *g, int v, int *visited)
{
    visited[v] = 1;
    adj_node_t *node = g->adj_list[v];
    while (node)
    {
        if (visited[node->vertex] == 0)
            dfs_util(g, node->vertex, visited);

        node = node->next;
    }
}

int graph_is_connected(graph_t *g)
{
    if (g == NULL || g->num_vertices == 0)
        return 1;

    if (g->num_vertices == 1)
        return 1;

    int *visited = calloc(g->num_vertices, sizeof(int));
    if (visited == NULL)
        return 0;

    dfs_util(g, 0, visited);

    for (int i = 0; i < g->num_vertices; i++)
    {
        if (visited[i] == 0)
        {
            free(visited);
            return 0;
        }
    }

    free(visited);
    return 1;
}

void graph_free(graph_t *g)
{
    if (g == NULL)
        return;

    for (int i = 0; i < g->num_vertices; i++)
    {
        adj_node_t *node = g->adj_list[i];
        while (node)
        {
            adj_node_t *tmp = node;
            node = node->next;
            free(tmp);
        }
    }

    free(g->adj_list);
    free(g);
}

int graph_create_dot(graph_t *g, const char *filename)
{
    if (g == NULL || filename == NULL)
        return ERROR_IO;

    FILE *f = fopen(filename, "w");
    if (f == NULL)
        return ERROR_IO;

    fprintf(f, "digraph G {\n");
    fprintf(f, "    bgcolor=black;\n");
    fprintf(f, "    node [shape=circle, style=filled, fillcolor=white, fontcolor=black, fontsize=14];\n");

    for (int u = 0; u < g->num_vertices; u++)
    {
        fprintf(f, "    %d;\n", u + 1);
    }

    for (int u = 0; u < g->num_vertices; u++)
    {
        adj_node_t *node = g->adj_list[u];
        while (node)
        {
            int v = node->vertex;
            if (u == v)
                fprintf(f, "    %d -> %d [dir=forward, color=white, penwidth=2, style=bold];\n", u + 1, v + 1);
            else if (u < v)
                fprintf(f, "    %d -> %d [dir=none, color=white, penwidth=2];\n", u + 1, v + 1);
            node = node->next;
        }
    }

    fprintf(f, "}\n");
    fclose(f);
    return ERROR_OK;
}

int graph_add_vertex(graph_t **g)
{
    if (g == NULL)
        return ERROR_IO;

    if (*g == NULL)
    {
        *g = graph_create(1);
        if (*g == NULL)
            return ERROR_MEM;
        return ERROR_OK;
    }

    graph_t *old_g = *g;
    int old_n = old_g->num_vertices;
    int new_n = old_n + 1;

    adj_node_t **new_adj_list = realloc(old_g->adj_list, new_n * sizeof(adj_node_t *));
    if (new_adj_list == NULL)
        return ERROR_MEM;

    old_g->adj_list = new_adj_list;
    old_g->adj_list[old_n] = NULL;
    old_g->num_vertices = new_n;

    return ERROR_OK;
}

graph_t *graph_generate_random(int n)
{
    if (n <= 0)
        return NULL;

    graph_t *g = graph_create(n);
    if (g == NULL)
        return NULL;

    srand((unsigned int)time(NULL));

    int max_edges = n * (n - 1) / 2;
    int target_edges = max_edges / 2;

    int added = 0;
    while (added < target_edges)
    {
        int u = rand() % n;
        int v = rand() % n;
        if (u == v)
            continue;

        added += graph_add_edge(g, u, v, 1);
    }

    return g;
}

size_t graph_memory_usage(const graph_t *g)
{
    if (g == NULL)
        return 0;

    size_t total = sizeof(graph_t);
    total += g->num_vertices * sizeof(adj_node_t *);

    int edge_count = 0;
    for (int i = 0; i < g->num_vertices; i++)
    {
        adj_node_t *node = g->adj_list[i];
        while (node)
        {
            edge_count++;
            node = node->next;
        }
    }

    total += edge_count * sizeof(adj_node_t);

    return total;
}

int statistic(void)
{
    printf("Запуск замеров производительности...\n");
    int sizes[] = {50, 100, 500, 1000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\n");
    printf("  +--------------------------------------------------+\n");
    printf("  |   Кол-во вершин | Время (сек) | Память (байт)    |\n");
    printf("  +--------------------------------------------------+\n");

    for (int i = 0; i < num_sizes; i++)
    {
        int n = sizes[i];
        graph_t *test_graph = graph_generate_random(n);
        if (test_graph == NULL)
        {
            printf("  | %10d | Ошибка выделения памяти\n", n);
            graph_free(test_graph);
            return ERROR_MEM;
        }

        clock_t start = clock();
        graph_is_connected(test_graph);
        clock_t end = clock();

        double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
        size_t mem_bytes = graph_memory_usage(test_graph);

        printf("  |      %10d | %11.6f | %10zu       |\n", n, time_spent, mem_bytes);

        graph_free(test_graph);
    }

    printf("  +--------------------------------------------------+\n");

    return ERROR_OK;
}