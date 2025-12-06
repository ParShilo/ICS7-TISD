#include <stdio.h>
#include <math.h>
#include "errors.h"
#include "defines.h"
#include "print.h"

void print_error(int rc)
{
    switch (rc)
    {
        case ERROR_IO:
            printf("Ошибка ввода!!!\n");
            break;
        case ERROR_MEM:
            printf("Ошибка памяти!!!\n");
            break;
        case ERROR_OVEFLOW:
            printf("Ошибка переполнения очереди!!!\n");
            break;
        case ERROR_EMPTY:
            printf("Ошибка удаления из пустой очереди!!!\n");
            break;
    }
}

void print_menu(void)
{
    printf("\n");
    printf("    +-----------------------------------------------------------+\n");
    printf("    |                           МЕНЮ                            |\n");
    printf("    +-----------------------------------------------------------+\n");
    printf("    | 1 - Очередь-массив: Добавить элемент.                     |\n");
    printf("    | 2 - Очередь-массив: Удалить элемент.                      |\n");
    printf("    | 3 - Очередь-массив: Вывести очередь.                      |\n");
    printf("    | 4 - Очередь-массив: Моделирование очереди.                |\n");
    printf("    |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("    | 5 - Очередь-список: Добавить элемент.                     |\n");
    printf("    | 6 - Очередь-список: Удалить элемент.                      |\n");
    printf("    | 7 - Очередь-список: Вывести очередь.                      |\n");
    printf("    | 8 - Очередь-список: Вывод использованных адресов.         |\n");
    printf("    | 9 - Очередь-список: Моделирование очереди.                |\n");
    printf("    |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("    | 10 - Замерный эксперимент.                                |\n");
    printf("    | 11 - Изменить параметры моделирования.                    |\n");
    printf("    |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("    | 0 - Выход из программы.                                   |\n");
    printf("    +-----------------------------------------------------------+\n");
    printf("Выбор операции: ");
}

// Функция вывода статистики
void print_statistics(const statistics_t *stats)
{
    printf("\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|                    ФИНАЛЬНАЯ СТАТИСТИКА                   |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| Общее время моделирования: %-25.2f е.в. |\n", stats->total_modeling_time);
    printf("| Время простоя аппарата:    %-25.2f е.в. |\n", stats->device_idle_time);
    printf("| Коэффициент простоя:     %-28.2f%%    |\n", (stats->device_idle_time / stats->total_modeling_time) * 100);
    printf("|                                                           |\n");
    printf("| Заявки 1 типа:                                            |\n");
    printf("|   - вошедшие:            %-32d |\n", stats->total_requests_type1);
    printf("|   - вышедшие:            %-32d |\n", stats->served_requests_type1);
    printf("|                                                           |\n");
    printf("| Обращения заявки 2 типа: %-32d |\n", stats->total_requests_type2);
    printf("|                                                           |\n");
    printf("| Длина очереди:                                            |\n");
    printf("|   - максимальная:         %-31d |\n", stats->max_queue_length);
    printf("|   - средняя:              %-31.2f |\n", stats->total_queue_length / stats->measurements_count);
    printf("|                                                           |\n");
    printf("| Среднее время в очереди: %-27.2f е.в. |\n", stats->total_queue_time / stats->served_requests_type1);
    calculate_accuracy(stats);
}

// Функция вывода промежуточной статистики
void print_intermediate_stats(const statistics_t *stats, int queue_size, int step)
{
    printf("\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|          СТАТИСТИКА ПОСЛЕ %4d ЗАЯВОК 1 ТИПА              |\n", step);
    printf("+-----------------------------------------------------------+\n");
    printf("| Текущая длина очереди:   %-32d |\n", queue_size);
    printf("|                                                           |\n");
    printf("| Средняя длина очереди:         %-26.2f |\n", stats->total_queue_length / stats->measurements_count);
    printf("|                                                           |\n");
    printf("| Заявки 1 типа:                                            |\n");
    printf("|   - вошедшие:            %-32d |\n", stats->total_requests_type1);
    printf("|   - вышедшие:            %-32d |\n", stats->served_requests_type1);
    printf("|                                                           |\n");
    if (stats->served_requests_type1 > 0)
        printf("| Среднее время в очереди:  %-31.2f |\n", stats->total_queue_time / stats->served_requests_type1);
    else
        printf("| Среднее время в очереди:  %-31s |\n", "0.00");
    printf("+-----------------------------------------------------------+\n");
    printf("\n");
}

// Функция расчета теоретических значений и погрешности
void calculate_accuracy(const statistics_t *stats)
{
    // Расчёт теоретических значений
    double avg_arrival_interval = (q1_arrival_min + q1_arrival_max) / 2.0;
    double avg_service_time_type1 = (q1_service_min + q1_service_max) / 2.0;
    double avg_service_time_type2 = (q2_service_min + q2_service_max) / 2.0;
    
    // Теоретическое время моделирования
    double expected_arrival_time = avg_arrival_interval * stats->total_requests_type1;
    double expected_service_time = (avg_service_time_type1 * stats->served_requests_type1) + (avg_service_time_type2 * stats->total_requests_type2);
    
    double expected_modeling_time;
    if (avg_service_time_type1 > avg_arrival_interval)
        expected_modeling_time = expected_service_time;
    else
        expected_modeling_time = expected_arrival_time;
    
    // Погрешность по времени
    double error_time = 100.0 * fabs(stats->total_modeling_time - expected_modeling_time) / expected_modeling_time;
    
    // Погрешность по входу
    double expected_entered_by_time = stats->total_modeling_time / avg_arrival_interval;
    double error_entered = 100.0 * fabs(stats->total_requests_type1 - expected_entered_by_time) / expected_entered_by_time;
    
    // Погрешность по выходу
    double actual_service_time = stats->total_modeling_time - stats->device_idle_time;
    double expected_service_by_time = (avg_service_time_type1 * stats->served_requests_type1) + (avg_service_time_type2 * stats->total_requests_type2);
    double error_service = 100.0 * fabs(actual_service_time - expected_service_by_time) / expected_service_by_time;
    
    printf("+-----------------------------------------------------------+\n");
    printf("|                ПРОВЕРКА ТОЧНОСТИ МОДЕЛИ                   |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| Теоретическое время моделирования:   %-15.2f е.в. |\n", expected_modeling_time);
    printf("| Фактическое время моделирования:     %-15.2f е.в. |\n", stats->total_modeling_time);
    printf("| Погрешность по времени:              %-15.2f%%     |\n", error_time);
    printf("|                                                           |\n");
    printf("| Проверка по ВХОДУ (по поступлению):                       |\n");
    printf("|   Ожидаемое число заявок:           %-15.2f       |\n", expected_entered_by_time);
    printf("|   Фактическое число заявок:         %-15d       |\n", stats->total_requests_type1);
    printf("|   Погрешность:                      %-15.2f%%      |\n", error_entered);
    printf("|                                                           |\n");
    printf("| Проверка по ВЫХОДУ (по обслуживанию):                     |\n");
    printf("|   Ожидаемое время обслуживания:      %-15.2f е.в. |\n", expected_service_by_time);
    printf("|   Фактическое время обслуживания:    %-15.2f е.в. |\n", actual_service_time);
    printf("|   Погрешность:                       %-15.2f%%     |\n", error_service);
    printf("+-----------------------------------------------------------+\n");
}