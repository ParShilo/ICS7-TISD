#ifndef DEFINES_H__
#define DEFINES_H__

#define MAX_QUEUE_SIZE 1000

// Объявления глобальных переменных
extern double q1_arrival_min;
extern double q1_arrival_max;
extern double q1_service_min;
extern double q1_service_max;
extern double q2_service_min;
extern double q2_service_max;

// Структура для заявки с временем поступления
typedef struct
{
    int type;
    double arrival_time;
} request_t;

// Структура для очереди на массиве
typedef struct
{
    request_t data[MAX_QUEUE_SIZE];
    int pin;
    int pout;
    int size;
} array_queue;

// Структура для элемента списка
typedef struct node
{
    request_t request;
    struct node *next;
} node_t;

// Структура для очереди на списке
typedef struct
{
    node_t *pin;
    node_t *pout;
    int size;
} list_queue;

// Типы заявок
#define REQUEST_TYPE_1 1
#define REQUEST_TYPE_2 2

// Структура для статистики
typedef struct
{
    int total_requests_type1;
    int served_requests_type1;
    int total_requests_type2;
    double total_queue_time;
    double total_modeling_time;
    double device_idle_time;
    int max_queue_length;
    double total_queue_length;
    int measurements_count;
} statistics_t;

// Структура для состояния системы
typedef struct
{
    double time;
    double next_arrival_type1;
    double next_service_end;
    int device_busy;
    request_t current_request;   
    double service_start_time;
} system_state_t;

#endif