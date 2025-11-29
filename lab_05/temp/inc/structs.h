#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

// Узел списка
typedef struct node_t {
    int type;           // 1 - заявка 1-го типа, 2 - заявка 2-го типа
    double arrival_time; // время прихода в очередь
    struct node_t *next;
} node_t;

// Очередь (общая структура)
typedef struct {
    // Для массива
    int array_pin;      // индекс хвоста
    int array_pout;     // индекс головы
    int array_len;      // текущая длина
    
    // Для списка  
    node_t *list_pin;   // указатель на хвост
    node_t *list_pout;  // указатель на голову
    int list_len;       // текущая длина
    
    // Общая статистика
    int in_num;         // количество вошедших заявок 1-го типа
    int state;          // сумма длин для средней длины
    int max_len;        // максимальная длина очереди
    double total_stay_time; // общее время пребывания
} queue_t;

// Обслуживающий аппарат
typedef struct {
    double time;        // текущее время ОА
    double downtime;    // время простоя
    int triggering;     // количество срабатываний
    int processed_count_1; // обработано заявок 1-го типа
    int type2_requests; // обращения заявки 2-го типа
} oa_t;

// Для отслеживания памяти (только для списка)
typedef struct mem_t {
    void *address;
    int busy;
    struct mem_t *next;
} mem_t;

uint64_t tick(void);
double get_time(int t1, int t2);

#endif