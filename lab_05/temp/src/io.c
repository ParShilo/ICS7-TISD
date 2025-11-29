#include "io.h"
#include <stdio.h>
#include <math.h>

void print_menu(void)
{
    printf("\n=== МЕНЮ ===\n");
    printf("1. Моделирование очереди (массив)\n");
    printf("2. Моделирование очереди (список)\n");
    printf("3. Показать адреса памяти (для списка)\n");
    printf("4. Сравнительный анализ\n");
    printf("0. Выход\n");
    printf("Выбор: ");
}

void print_intermediate_stats(int processed_count, int len, double avg_len, 
                             int in_num, double avg_stay_time)
{
    printf("\n+-------------------------------------------+\n");
    printf("| %-30s | %-8d |\n", "Обработано заявок 1-го типа", processed_count);
    printf("| %-30s | %-8d |\n", "Текущая длина очереди", len);
    printf("| %-30s | %-8.2f |\n", "Средняя длина очереди", avg_len);
    printf("| %-30s | %-8d |\n", "Вошедших заявок 1-го типа", in_num);
    printf("| %-30s | %-8.2f |\n", "Среднее время пребывания", avg_stay_time);
    printf("+-------------------------------------------+\n");
}

void print_final_stats(double total_time, double downtime, int in_count_1, 
                      int processed_1, int type2_requests, double error)
{
    printf("\n=== ИТОГОВАЯ СТАТИСТИКА ===\n");
    printf("Общее время моделирования: %.2f е.в.\n", total_time);
    printf("Время простоя ОА: %.2f е.в.\n", downtime);
    printf("Вошедших заявок 1-го типа: %d\n", in_count_1);
    printf("Обработанных заявок 1-го типа: %d\n", processed_1);
    printf("Обращений заявки 2-го типа: %d\n", type2_requests);
    printf("Погрешность: %.2f%%\n", error);
}