/*

Смоделировать операцию умножения действительного числа на действительное число в форме ±m.n Е ±K, 
где суммарная длина мантиссы первого сомножителя (m+n) - до 35 значащих цифр, второго – до 40 
значащих цифр, а величина порядка K - до 5 цифр. Результат выдать в форме
±0.m1 Е ±K1, где m1 – до 40 значащих цифр, а K1 - до 5 цифр.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "err_def.h"
#include "num_func.h"

int main(void)
{
    int rc = OK;
    char input_1[MAX_INPUT_LEN + 1], input_2[MAX_INPUT_LEN + 1];
    number number_1, number_2, result;

    // Вывод приглашения
    printf("~~~ ПРОГРАММА ВЫЧИСЛЯЕТ РЕЗУЛЬТАТ УМНОЖЕНИЯ ДВУХ ДЕЙСТВИТЕЛЬНЫХ ЧИСЕЛ ~~~\n\n");
    printf("Первое вводимое число ограничено %d цифрами мантиссы.\n", MAX_LEN_FIRST_LINE);
    printf("Второе вводимое число ограничено %d цифрами мантиссы.\n", MAX_LEN_SECOND_LINE);
    printf("Число вводится в формате: ±m.nЕ±K; Степень ограничена %d цифрами.\n\n", MAX_LEN_EXPONENT);

    // Ввод первого числа
    printf("Введите первое число:\n");
    printf("> ---------1---------2---------3---------4---------5---------6\n>");
    if (fgets(input_1, MAX_INPUT_LEN, stdin) == NULL)
        rc = ERROR_INPUT;
    else
    {
        // Парсинг первого числа
        rc = parse_number(input_1, &number_1, MAX_LEN_FIRST_LINE);
        if (rc == OK)
        {
            printf("Введено: ");
            print_number(&number_1);
        }
    }
        
    // Ввод второго числа
    if (rc == OK)
    {
        printf("\nВведите второе число:\n");
        printf("> ---------1---------2---------3---------4---------5---------6\n>");
        if (fgets(input_2, MAX_INPUT_LEN, stdin) == NULL)
            rc = ERROR_INPUT;
        else
        {
            // Парсинг второго числа
            rc = parse_number(input_2, &number_2, MAX_LEN_SECOND_LINE);
            if (rc == OK)
            {
                printf("Введено: ");
                print_number(&number_2);
            }
        }
    }

    // Алгоритм умножения
    if (rc == OK)
        rc = multiplication(&number_1, &number_2, &result);

    // Вывод результата
    if (rc == OK)
    {
        printf("\nРезультат умножения: \n");
        printf(">  0.---------1---------2---------3---------4---------5---------6\n>>");
        print_number(&result);
    }
    else if (rc == ERROR_INPUT)
        printf("\nОшибка!!! Невалидный ввод.\n");
    else if (rc == ERROR_EMPTY)
        printf("\nОшибка!!! Пустой ввод.\n");
    else if (rc == ERROR_LONG_EXPONENTA)
        printf("\nОшибка!!! Слишком большая экспонента.\n");
    else if (rc == ERROR_LONG_MANTISSA)
        printf("\nОшибка!!! Слишком большая мантисса.\n");
    else if (rc == ERROR_OVERFLOW)
        printf("\nОшибка!!! Слишком большой порядок (машинная бесконечность).\n");
    else if (rc == ERROR_UNDERFLOW)
        printf("\nОшибка!!! Слишком маленький порядок (машинный нуль).\n");

    return rc;
}