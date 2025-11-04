#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "country.h"
#include "print.h"
#include "proc_country.h"

// Функция для загрузки первоначального массива стран из файла
int load_data(country_t countries[], size_t *number_countries)
{
    char filename[MAX_STRING_LEN + 1];
    
    printf("Введите название файла для загрузки данных: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL)
        return ERROR_INPUT;
    
    filename[strcspn(filename, "\n")] = '\0';
    size_t len = strlen(filename);
    
    if (len == 0)
        return ERROR_INPUT;

    FILE *file = fopen(filename, "rt");
    if (file == NULL) 
        return ERROR_FILE;

    printf("Загрузка данных из файла %s\n", filename);

    *number_countries = 0;

    // Временные переменные
    country_t *temp_country = NULL;
    char name[MAX_STRING_LEN + 1], capital[MAX_STRING_LEN + 1], continent[MAX_STRING_LEN + 1];
    int visa_required, flight_time, min_cost, tourism_type;
    int objects_count, object_type;
    int season, air_temp, water_temp;
    int sport_type;

    while (*number_countries <= MAX_COUNTRIES && !feof(file)) 
    {
        temp_country = &countries[*number_countries];
        
        // Чтение с проверкой количества полей
        int fields_read = fscanf(file, "%s %s %s %d %d %d %d", 
                  name, capital, continent,
                  &visa_required, &flight_time, 
                  &min_cost, &tourism_type);
        
        if (feof(file) && *number_countries == 0)
        {
            fclose(file);
            return ERROR_EMPTY_FILE;
        }
    
        if (fields_read != 7)
        {
            fclose(file);
            return ERROR_DATA;
        }

        // Валидация строковых полей
        if (strlen(name) > MAX_STRING_LEN || strlen(capital) > MAX_STRING_LEN || strlen(continent) > MAX_STRING_LEN)
        {
            fclose(file);
            return ERROR_DATA;
        }

        // Валидация числовых полей
        if ((visa_required != 0 && visa_required != 1) || flight_time < 0 || min_cost < 0 || tourism_type < 0 || tourism_type > 2)
        {
            fclose(file);
            return ERROR_DATA;
        }

        // Копирование
        strcpy(temp_country->name, name);
        strcpy(temp_country->capital, capital);
        strcpy(temp_country->continent, continent);
        temp_country->visa_required = visa_required;
        temp_country->flight_time = flight_time;
        temp_country->min_cost = min_cost;
        temp_country->tourism = (tourism_t)tourism_type;

        // Чтение вариантой части с валидацией
        switch (temp_country->tourism) 
        {
            case EXCURSION:
                {
                    if (fscanf(file, "%d %d", &objects_count, &object_type) != 2 || objects_count <= 0 || object_type < 0 || object_type > 2)
                    {
                        fclose(file);
                        return ERROR_DATA;
                    }
                    
                    temp_country->tourism_details.excursion.objects_count = objects_count;
                    temp_country->tourism_details.excursion.object_type = (object_t)object_type;
                }
                break;
                
            case BEACH:
                {
                    if (fscanf(file, "%d %d %d", &season, &air_temp, &water_temp) != 3 || season < 0 || season > 3)
                    {
                        fclose(file);
                        return ERROR_DATA;
                    }

                    temp_country->tourism_details.beach.season = (season_t)season;
                    temp_country->tourism_details.beach.air_temp = air_temp;
                    temp_country->tourism_details.beach.water_temp = water_temp;
                }
                break;
                
            case SPORT:
                {
                    if (fscanf(file, "%d", &sport_type) != 1 || sport_type < 0 || sport_type > 2)
                    {
                        fclose(file);
                        return ERROR_DATA;
                    }

                    temp_country->tourism_details.sport.sport_type = (sport_t)sport_type;
                }
                break;
        }

        (*number_countries)++;
        if (*number_countries > MAX_COUNTRIES)
        {
            fclose(file);
            return ERROR_OVERFLOW;
        }

        
        int c;
        while ((c = fgetc(file)) != '\n' && c != EOF);
    }
    
    if (*number_countries == 0)
    {
        fclose(file);
        return ERROR_EMPTY_FILE;
    }
    
    printf("Итого загружено %zu корректных записей из файла %s\n", *number_countries, filename);
    fclose(file);
    return OK;
}

// Функция для сохранения данных в файл
int save_data(const country_t countries[], const size_t number_countries)
{
    char filename[MAX_STRING_LEN + 1];
    
    printf("Введите название файла для сохранения данных: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL)
        return ERROR_INPUT;
    
    filename[strcspn(filename, "\n")] = '\0';
    size_t len = strlen(filename);
    
    if (len == 0)
        return ERROR_INPUT;

    FILE *file = fopen(filename, "wt");
    if (file == NULL) 
        return ERROR_FILE;

    printf("Сохранение данных в файл %s\n", filename);

    for (size_t i = 0; i < number_countries; i++)
    {
        const country_t *c = &countries[i];
        
        // Запись основных полей
        fprintf(file, "%s %s %s %d %d %d %d",
                c->name, c->capital, c->continent,
                c->visa_required, c->flight_time, 
                c->min_cost, c->tourism);

        // Запись вариантной части
        switch (c->tourism) 
        {
            case EXCURSION:
                fprintf(file, " %d %d",
                        c->tourism_details.excursion.objects_count,
                        c->tourism_details.excursion.object_type);
                break;
                
            case BEACH:
                fprintf(file, " %d %d %d",
                        c->tourism_details.beach.season,
                        c->tourism_details.beach.air_temp,
                        c->tourism_details.beach.water_temp);
                break;
                
            case SPORT:
                fprintf(file, " %d",
                        c->tourism_details.sport.sport_type);
                break;
        }
        
        if (i != number_countries - 1)
            fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Успешно сохранено %zu записей в файл %s\n", number_countries, filename);
    return OK;
}

// Функции для добавления страны
int add_record(country_t countries[], size_t *number_countries)
{
    if (*number_countries >= MAX_COUNTRIES)
        return ERROR_OVERFLOW;

    country_t *c = &countries[*number_countries];
    
    // Временные переменные
    char name[MAX_STRING_LEN + 2], capital[MAX_STRING_LEN + 2], continent[MAX_STRING_LEN + 2];
    int visa_required, flight_time, min_cost, tourism_type;
    int objects_count, object_type;
    int season, air_temp, water_temp;
    int sport_type;

    // Ввод и валидация названия страны
    printf("Название страны: ");
    if (fgets(name, sizeof(name), stdin) == NULL || strlen(name) == 0)
        return ERROR_INPUT;
    name[strcspn(name, "\n")] = '\0';
    if (strlen(name) > MAX_STRING_LEN)
        return ERROR_INPUT;

    // Ввод и валидация столицы
    printf("Столица: ");
    if (fgets(capital, sizeof(capital), stdin) == NULL || strlen(capital) == 0)
        return ERROR_INPUT;
    capital[strcspn(capital, "\n")] = '\0';
    if (strlen(capital) > MAX_STRING_LEN)
        return ERROR_INPUT;

    // Ввод и валидация материка
    printf("Материк: ");
    if (fgets(continent, sizeof(continent), stdin) == NULL || strlen(continent) == 0)
        return ERROR_INPUT;
    continent[strcspn(continent, "\n")] = '\0';
    if (strlen(continent) > MAX_STRING_LEN)
        return ERROR_INPUT;

    // Ввод и валидация визы
    printf("Нужна виза (0-нет, 1-да): ");
    if (scanf("%d", &visa_required) != 1 || (visa_required != 0 && visa_required != 1))
        return ERROR_INPUT;

    // Ввод и валидация времени полета
    printf("Время полета (в часах): ");
    if (scanf("%d", &flight_time) != 1 || flight_time < 0)
        return ERROR_INPUT;

    // Ввод и валидация стоимости
    printf("Минимальная стоимость отдыха: ");
    if (scanf("%d", &min_cost) != 1 || min_cost < 0)
        return ERROR_INPUT;

    // Ввод и валидация типа туризма
    printf("Тип туризма (0-Экскурсионный, 1-Пляжный, 2-Спортивный): ");
    if (scanf("%d", &tourism_type) != 1 || tourism_type < 0 || tourism_type > 2)
        return ERROR_INPUT;

    // Очистка буфера после числового ввода
    while (getchar() != '\n');

    // Копирование валидированных данных
    strcpy(c->name, name);
    strcpy(c->capital, capital);
    strcpy(c->continent, continent);
    c->visa_required = visa_required;
    c->flight_time = flight_time;
    c->min_cost = min_cost;
    c->tourism = (tourism_t)tourism_type;

    // Ввод вариантной части с валидацией
    switch (c->tourism) 
    {
        case EXCURSION:
            printf("Количество объектов: ");
            if (scanf("%d", &objects_count) != 1 || objects_count < 0)
                return ERROR_INPUT;

            printf("Тип объекта (0-Природа, 1-Искусство, 2-История): ");
            if (scanf("%d", &object_type) != 1 || object_type < 0 || object_type > 2)
                return ERROR_INPUT;

            c->tourism_details.excursion.objects_count = objects_count;
            c->tourism_details.excursion.object_type = (object_t)object_type;
            break;

        case BEACH:
            printf("Сезон (0-Лето, 1-Зима, 2-Весна, 3-Осень): ");
            if (scanf("%d", &season) != 1 || season < 0 || season > 3)
                return ERROR_INPUT;

            printf("Температура воздуха: ");
            if (scanf("%d", &air_temp) != 1)
                return ERROR_INPUT;

            printf("Температура воды: ");
            if (scanf("%d", &water_temp) != 1)
                return ERROR_INPUT;

            c->tourism_details.beach.season = (season_t)season;
            c->tourism_details.beach.air_temp = air_temp;
            c->tourism_details.beach.water_temp = water_temp;
            break;

        case SPORT:
            printf("Вид спорта (0-горные лыжи, 1-серфинг, 2-восхождения): ");
            if (scanf("%d", &sport_type) != 1 || sport_type < 0 || sport_type > 2)
                return ERROR_INPUT;

            c->tourism_details.sport.sport_type = (sport_t)sport_type;
            break;
    }

    // Очистка буфера в конце
    while (getchar() != '\n');

    (*number_countries)++;
    printf("Страна успешно добавлена. Всего записей: %zu\n", *number_countries);
    
    return OK;
}

// Функция для удаления страны
int delete_country(country_t countries[], size_t *number_countries)
{
    if (*number_countries == 0)
    {
        printf("В массиве нет элементов.\n");
        return OK;
    }

    char name[MAX_STRING_LEN];
    printf("Введите название страны для удаления: ");
    if (fgets(name, sizeof(name), stdin) == NULL || strlen(name) == 0)
        return ERROR_INPUT;
    name[strcspn(name, "\n")] = '\0';
    if (strlen(name) > MAX_STRING_LEN)
        return ERROR_INPUT;
    
    char found = 0;

    for (size_t i = 0; i < *number_countries; i++) 
    {
        if (strcmp(countries[i].name, name) == 0) 
        {
            for (size_t j = i; j < *number_countries - 1; j++)
                countries[j] = countries[j + 1];

            (*number_countries)--;
            found = 1;
            printf("Страна удалена.\n");
            break;
        }
    }
    if (found == 0)
        printf("Страна не найдена.\n");

    return OK;
}

// Функция для поиска по спортивному туризму
int search_by_sport(const country_t countries[], const size_t number_countries)
{
    char continent[MAX_STRING_LEN + 1];
    int sport_type, max_cost;
        
    printf("Материк: ");
    if (fgets(continent, sizeof(continent), stdin) == NULL || strlen(continent) == 0)
        return ERROR_INPUT;
    continent[strcspn(continent, "\n")] = '\0';
    if (strlen(continent) > MAX_STRING_LEN)
        return ERROR_INPUT;
    
    printf("Вид спорта (0-Горные лыжи, 1-Сёрфинг, 2-Восхождения): ");
    if (scanf("%d", &sport_type) != 1 || sport_type < 0 || sport_type > 2)
        return ERROR_INPUT;
    
    printf("Максимальная стоимость: ");
    if (scanf("%d", &max_cost) != 1 || max_cost < 0)
        return ERROR_INPUT;
    
    // Очистка буфера
    while (getchar() != '\n');
    
    printf("\nРезультаты поиска:\n");
    int found = 0;
    for (size_t i = 0; i < number_countries; i++)
    {
        if (strcmp(countries[i].continent, continent) == 0 && countries[i].tourism == SPORT && countries[i].tourism_details.sport.sport_type == (sport_t)sport_type && countries[i].min_cost <= max_cost)
        {
            if (found == 0)
            {
                printf("Найдена страна(-ы): \n");
                print_head();
            }
            printf("[%2zu] ", i);
            print_country(&countries[i]);
            found = 1;
        }
    }
    
    if (!found)
        printf("Страны не найдены по заданным критериям.\n");
    else
        printf("+----+-----------------+-----------------+-----------------+------+------+---------+---------------+----+----------+\n");

    return OK;
}
