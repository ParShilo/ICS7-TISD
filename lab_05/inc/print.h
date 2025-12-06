#ifndef PRINT_H__
#define PRINT_H__

#include "defines.h"

// Функция вывода сообщения об ошибке
void print_error(int rc);
// Функция вывода меню
void print_menu(void);
// Функция вывода статистики
void print_statistics(const statistics_t *stats);
// Функция вывода промежуточной статистики
void print_intermediate_stats(const statistics_t *stats, int queue_size, int step);
// Функция расчета теоретических значений и погрешности
void calculate_accuracy(const statistics_t *stats);

#endif