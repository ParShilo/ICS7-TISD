#ifndef SIMULATE_H__
#define SIMULATE_H__

#include "defines.h"
#include "errors.h"
#include "arr.h"
#include "list.h"

// Типы заявок
#define REQUEST_TYPE_1 1
#define REQUEST_TYPE_2 2

// Структура для статистики
typedef struct
{
    int total_requests_type1;    // всего заявок 1 типа вошло
    int served_requests_type1;   // обслужено заявок 1 типа
    int total_requests_type2;    // всего обращений заявки 2 типа
    double total_queue_time;     // суммарное время в очереди
    double total_modeling_time;  // общее время моделирования
    double device_idle_time;     // время простоя аппарата
    int max_queue_length;        // максимальная длина очереди
    double total_queue_length;   // сумма длин очереди для усреднения
    int measurements_count;      // количество измерений для усреднения
} statistics_t;

// Структура для состояния системы
typedef struct
{
    double time;                 // текущее модельное время
    double next_arrival_type1;   // время следующего прихода заявки 1 типа
    double next_service_end;     // время окончания обслуживания
    int device_busy;             // занят ли аппарат (0 - свободен, 1 - занят)
    request_t current_request;   // <-- Изменено: теперь это структура
    double service_start_time;   // время начала обслуживания текущей заявки
} system_state_t;

// Прототипы функций моделирования
double generate_random_time(double min, double max);
int simulate_array_queue(int show_addresses);
int simulate_list_queue(int show_addresses);
void print_statistics(const statistics_t *stats, int is_final);
void print_intermediate_stats(const statistics_t *stats, int queue_size, int step); // <-- Добавлено
#endif