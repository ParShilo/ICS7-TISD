#ifndef STACK_H__
#define STACK_H__

#define MAX_SIZE 10000

// Структура для узла списка
typedef struct Node
{
    int data;
    struct Node* next;
} Node;

// Структура для стека на списке
typedef struct
{
    Node* top;
    int size;
} ListStack;

// Структура для стека на массиве
typedef struct
{
    int data[MAX_SIZE];
    int top;
} ArrayStack;

#endif