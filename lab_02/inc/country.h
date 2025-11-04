#ifndef COUNTRY_H__
#define COUNTRY_H__

#define MAX_COUNTRIES 1000
#define MAX_STRING_LEN 20

// Виды туризма
typedef enum{
    EXCURSION,
    BEACH,
    SPORT
} tourism_t;

// Виды объектов (экскурсия)
typedef enum {
    NATURE,
    ART,
    HISTORY
} object_t;

// Сезоны (пляж)
typedef enum {
    SUMMER,
    WINTER,
    SPRING,
    AUTUMN
} season_t;

// Виды спорта
typedef enum {
    SKIING,
    SURFING,
    CLIMBING
} sport_t;

// Основная структура
typedef struct {
    char name[MAX_STRING_LEN];
    char capital[MAX_STRING_LEN];
    char continent[MAX_STRING_LEN];
    int visa_required;
    int flight_time;
    int min_cost;
    tourism_t tourism;
    union {
        struct {
            int objects_count;
            object_t object_type;
        } excursion;
        struct {
            season_t season;
            int air_temp;
            int water_temp;
        } beach;
        struct {
            sport_t sport_type;
        } sport;
    } tourism_details;
} country_t;

// Структура для таблицы ключей
typedef struct {
    size_t index;
    int key; // ключ - минимальная стоимость
} key_t;

#endif