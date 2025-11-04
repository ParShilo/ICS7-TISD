/*
Лабораторная работа №2 Крылов Владислав ИУ7-32Б

Ввести список стран, содержащий название страны, столицу, материк,
необходимость наличия визы, время полета до страны, минимальную
стоимость отдыха, основной вид туризма:
1. Экскурсионный:
    a. Количество объектов
    b. Основной вид объектов (природа, искусство, история)
2. Пляжный:
    a. Основной сезон
    b. Температура воздуха и воды
3. Спортивный:
    a. Вид спорта (горные лыжи, серфинг, восхождения)
Вывести список стран на выбранном материке, где можно заняться
указанным видом спорта, со стоимостью отдыха меньше указанной
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "country.h"
#include "proc_country.h"
#include "sort_country.h"
#include "print.h"
#include "key.h"

// Изменения: удаление из пустого массива, время полёта в часах, вывод.

int main(void)
{
    int rc = OK;
    country_t countries[MAX_COUNTRIES + 1];
    key_t key_table[MAX_COUNTRIES];
    size_t number_countries = 0;
    int choice = 1;
    char file_flag = 0, key_flag = 0;

    // Вывод первоначальной информации
    print_structure_info();

    // Меню
    while (rc == OK && choice)
    {
        print_menu();
        if (scanf("%d", &choice) != 1 || choice > 14 || choice < 0)
        {
            rc = ERROR_INPUT;
            break;
        }
        while (getchar() != '\n');
        switch (choice) 
        {
            case 0:
                break;
            case 1:
                // Открытие файла для чтения
                printf("...Открытие файла для чтения...\n");
                rc = load_data(countries, &number_countries);
                file_flag = 1;
                break;
            case 2:
                if (!file_flag)
                    printf("Нет данных для использования, откройте файл.\n");
                // Вывод данных
                else
                {
                    printf("...Вывод данных о странах...\n");
                    print_data(countries, number_countries);
                }
                break;
            case 3:
                if (!file_flag)
                    printf("Нет данных для использования, откройте файл.\n");
                // Добавление страны
                else
                {
                    printf("...Добавление записи о стране...\n");
                    rc = add_record(countries, &number_countries);
                }
                break;
            case 4:
                if (!file_flag)
                    printf("Нет данных для использования, откройте файл.\n");
                // Удаление страны по названию
                else
                {
                    printf("...Удаление записи о стране...\n");
                    rc = delete_country(countries, &number_countries);
                }
                break;
            case 5:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                // Поиск по спорту
                else
                {
                    printf("...Поиск стран по типу спорта...\n");
                    rc = search_by_sport(countries, number_countries);
                }
                break;
            case 6:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                // Сортировка исходной таблицы пузыркём
                else
                {
                    printf("...Сортировка стран по минимальной стоимости методом пузырька...\n");
                    bubble_sort(countries, number_countries);
                }
                break;    
            case 7:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                else if (!key_flag)
                    printf("Ошибка: сначала инициализируйте таблицу ключей.\n");
                // Сортировка таблицы ключей пузыркём
                else
                {
                    printf("...Сортировка ключей методом пузырька...\n");
                    bubble_sort_key(key_table, number_countries);
                }
                break; 
            case 8:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                // Быстрая сортировка исходной таблицы
                else
                {
                    printf("...Быстрая сортировка стран по минимальной стоимости...\n");
                    quick_sort(countries, 0, number_countries - 1);
                }
                break;  
            case 9:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                else if (!key_flag)
                    printf("Ошибка: сначала инициализируйте таблицу ключей.\n");
                // Быстрая сортировка таблицы ключей
                else
                {
                    printf("...Быстрая сортировка ключей...\n");
                    quick_sort_key(key_table, 0, number_countries - 1);
                }
                break;  
            case 10:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                else if (!key_flag)
                    printf("Ошибка: сначала инициализируйте таблицу ключей.\n");
                else
                {   
                    printf("...Вывод таблицы стран через таблицу ключей...\n");
                    print_by_key(key_table, countries, number_countries);
                }
                break;      
            case 11:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                else
                {
                    printf("...Замер времени...\n");
                    test_sorting(countries, number_countries);
                }
                break;   
            case 12:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                else if (!key_flag)
                    printf("Ошибка: сначала инициализируйте таблицу ключей.\n");
                else
                {
                    printf("...Вывод таблицы ключей...\n");
                    print_key_table(key_table, number_countries);
                }
                break;
            case 13:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                else
                {
                    printf("...Инициализация таблицы ключей...\n");
                    initialize_key_table(key_table, countries, number_countries);
                    key_flag = 1;
                }
                break;
            case 14:
                if (!file_flag)
                    printf("Ошибка: сначала загрузите данные из файла.\n");
                else
                {
                    printf("...Сохранение данных в файл...\n");
                    rc = save_data(countries, number_countries);
                }
                break;
        }
    }
    
    if (rc == ERROR_INPUT)
        printf("\033[91mОшибка ввода данных.\033[0m\n");
    else if (rc == ERROR_DATA)
        printf("\033[91mОшибка чтения данных.\033[0m\n");
    else if (rc == ERROR_FILE)
        printf("\033[91mОшибка чтения файла.\033[0m\n");
    else if (rc == ERROR_EMPTY_FILE)
        printf("\033[91mОшибка чтения пустого файла.\033[0m\n");
    else if (rc == ERROR_OVERFLOW)
        printf("\033[91mОшибка превышения количества стран.\033[0m\n");

    return rc;
}
