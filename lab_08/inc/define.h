#ifndef DEFINE_H__
#define DEFINE_H__

// Узел списка смежности
typedef struct adj_node
{
    int vertex;
    struct adj_node *next;
} adj_node_t;

// Граф: список смежности
typedef struct graph
{
    int num_vertices;
    adj_node_t **adj_list;
} graph_t;

#endif