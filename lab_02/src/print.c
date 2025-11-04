#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "country.h"
#include "print.h"

// Главное меню
void print_menu(void) 
{
    printf("\n");
    printf("                        +-----------------------------------------------------------+\n");
    printf("                        |                 Меню управления странами                  |\n");
    printf("                        +-----------------------------------------------------------+\n");
    printf("                        | 0 - Выход.                                                |\n");
    printf("                        | 1 - Загрузить данные из файла.                            |\n");
    printf("                        | 2 - Вывести исходную таблицу.                             |\n");
    printf("                        | 3 - Добавить запись.                                      |\n");
    printf("                        | 4 - Удалить запись.                                       |\n");
    printf("                        | 5 - Поиск стран для спортивного туризма.                  |\n");
    printf("                        | 6 - Сортировка исходной таблицы (пузырьком).              |\n");
    printf("                        | 7 - Сортировка таблицы ключей (пузырьком).                |\n");
    printf("                        | 8 - Сортировка исходной таблицы (быстрая).                |\n");
    printf("                        | 9 - Сортировка таблицы ключей (быстрая).                  |\n");
    printf("                        | 10 - Вывод через таблицу ключей.                          |\n");
    printf("                        | 11 - Сравнение эффективности алгоритмов.                  |\n");
    printf("                        | 12 - Вывод таблицы ключей.                                |\n");
    printf("                        | 13 - Инициализировать таблицу ключей.                     |\n");
    printf("                        | 14 - Сохранить данные в файл.                             |\n");
    printf("                        +-----------------------------------------------------------+\n");
    printf("Выбор: ");
}

// Функции вывода одной страны
void print_country(const country_t *c) 
{
    // Форматированный вывод с фиксированной шириной полей
    printf("| %-15s | %-15s | %-15s | %-6s | %4d | %7d | ",
           c->name, c->capital, c->continent,
           c->visa_required ? "Да" : "Нет ",
           c->flight_time, c->min_cost);

    switch (c->tourism) 
    {
        case EXCURSION:
            printf("%-15s | %2d | %-8s |\n",
                   "Экскурсионный",
                   c->tourism_details.excursion.objects_count,
                   (c->tourism_details.excursion.object_type == NATURE) ? "Природа" :
                   (c->tourism_details.excursion.object_type == ART) ? "Искусство" : "История");
            break;
        case BEACH:
            printf("%-15s | %-5s | %3d | %3d |\n",
                   "Пляжный      ",
                   (c->tourism_details.beach.season == SUMMER) ? "Лето" :
                   (c->tourism_details.beach.season == WINTER) ? "Зима" :
                   (c->tourism_details.beach.season == SPRING) ? "Весна" : "Осень",
                   c->tourism_details.beach.air_temp,
                   c->tourism_details.beach.water_temp);
            break;
        case SPORT:
            printf("%-15s | %-15s |\n",
                   "Спортивный   ",
                   (c->tourism_details.sport.sport_type == SKIING) ? "Горные лыжи  " :
                   (c->tourism_details.sport.sport_type == SURFING) ? "Сёрфинг     " : "Восхождения  ");
            break;
    }
}

// Функция вывода шапки таблицы
void print_head(void)
{
    printf("+----+-----------------+-----------------+-----------------+------+------+---------+---------------+----+----------+\n");
    printf("| №  | Страна          | Столица         | Материк         | Виза | Час. | Стоим.  | Тип туризма   | Доп. данные   |\n");
    printf("+----+-----------------+-----------------+-----------------+------+------+---------+---------------+----+----------+\n");
}

// Функция вывода таблицы стран
void print_data(const country_t countries[], const size_t number_countries) 
{
    if (number_countries == 0)
    {
        printf("Таблица пуста.\n");
        return;
    }

    printf("\nИсходная таблица (%zu записей):\n", number_countries);
    print_head();
    for (size_t i = 0; i < number_countries; i++)
    {
        printf("[%2zu] ", i);
        print_country(&countries[i]);
    }
    printf("+----+-----------------+-----------------+-----------------+------+------+---------+---------------+----+----------+\n");
}

// Функция вывода таблицы ключей
void print_key_table(const key_t key_table[], const size_t number_countries)
{
    if (number_countries == 0)
    {
        printf("Таблица ключей пуста.\n");
        return;
    }
    
    printf("\nТаблица ключей (%zu записей):\n", number_countries);
    printf("+-----+-------+\n");
    printf("| Инд | Ключ  |\n");
    printf("+-----+-------+\n");
    for (size_t i = 0; i < number_countries; i++)
        printf("| %3zu | %5d |\n", key_table[i].index, key_table[i].key);
    printf("+-----+-------+\n");
}

// Функция вывода таблицы стран через таблицу ключей
void print_by_key(const key_t key_table[], const country_t countries[], const size_t number_countries)
{
    printf("\nИсходная таблица через таблицу ключей(%zu записей):\n", number_countries);
    print_head();
    for (size_t i = 0; i < number_countries; i++)
    {
        printf("[%2zu] ", key_table[i].index);
        print_country(&countries[key_table[i].index]);
    }
    printf("+----+-----------------+-----------------+-----------------+------+------+---------+---------------+----+----------+\n");
}

// Функция вывода информации о стране
void print_structure_info(void)
{
    printf("\n");
    printf("                        +-----------------------------------------------------------+\n");
    printf("                        |              Информация о структуре данных                |\n");
    printf("                        +-----------------------------------------------------------+\n");
    printf("                        | Страны имеет следующие характеристики:                    |\n");
    printf("                        |                                                           |\n");
    printf("                        | • Название страны:       строка до %d символов            |\n", MAX_STRING_LEN);
    printf("                        | • Столица:               строка до %d символов            |\n", MAX_STRING_LEN);
    printf("                        | • Материк:               строка до %d символов            |\n", MAX_STRING_LEN);
    printf("                        | • Виза:                  0-не нужна, 1-нужна              |\n");
    printf("                        | • Время полета:          целое число (часы)               |\n");
    printf("                        | • Минимальная стоимость: целое число                      |\n");
    printf("                        | • Тип туризма:           0-Экскурсионный, 1-Пляжный,      |\n");
    printf("                        |                          2-Спортивный                     |\n");
    printf("                        |                                                           |\n");
    printf("                        | Дополнительные поля по типам туризма:                     |\n");
    printf("                        |                                                           |\n");
    printf("                        | • ЭКСКУРСИОННЫЙ (0):                                      |\n");
    printf("                        |   - Количество объектов: > 0                              |\n");
    printf("                        |   - Тип объекта: 0-Природа, 1-Искусство, 2-История        |\n");
    printf("                        |                                                           |\n");
    printf("                        | • ПЛЯЖНЫЙ (1):                                            |\n");
    printf("                        |   - Сезон: 0-Лето, 1-Зима, 2-Весна, 3-Осень               |\n");
    printf("                        |   - Температура воздуха: целое число                      |\n");
    printf("                        |   - Температура воды: целое число                         |\n");
    printf("                        |                                                           |\n");
    printf("                        | • СПОРТИВНЫЙ (2):                                         |\n");
    printf("                        |   - Вид спорта: 0-Горные лыжи, 1-Сёрфинг, 2-Восхождения   |\n");
    printf("                        |                                                           |\n");
    printf("                        | Максимальное количество стран: %d                      |\n", MAX_COUNTRIES);
    printf("                        +-----------------------------------------------------------+\n");
}