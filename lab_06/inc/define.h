#ifndef DEFINE_H__
#define DEFINE_H__

#define MAX_STRING_SIZE 10000

typedef struct node_t 
{
    char data;
    int count;
    struct node_t *left, *right;
} node_t;

#endif