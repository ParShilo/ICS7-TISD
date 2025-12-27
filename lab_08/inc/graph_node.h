#ifndef GRAPH_NODE_H__
#define GRAPH_NODE_H__

#include <stdio.h>
#include "define.h"

graph_t *graph_create(int n);
int graph_add_edge(graph_t *g, int u, int v, int quiet);
int graph_add_vertex(graph_t **g);
int graph_is_connected(graph_t *g);
void graph_free(graph_t *g);
int graph_create_dot(graph_t *g, const char *filename);
graph_t *graph_generate_random(int n);
size_t graph_memory_usage(const graph_t *g);
int statistic(void);

#endif