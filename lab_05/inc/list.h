#ifndef LIST_H__
#define LIST_H__

// Структура для элемента списка
typedef struct node
{
    int data;
    float arrival_time;
    struct node *next;
} node_t;

// Структура для очереди на списке
typedef struct
{
    node_t *front;
    node_t *rear;
    int size;
} list_queue;

// Прототипы функций для списка
void init_list_queue(list_queue *q);
int is_list_queue_empty(list_queue *q);
int enqueue_list(list_queue *q, int value, float arrival_time, int show_address);
int dequeue_list(list_queue *q, int show_address);
void print_list_queue(list_queue *q);
void print_list_addresses(void);  // Добавлена эта функция

#endif