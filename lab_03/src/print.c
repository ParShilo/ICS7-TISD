#include <stdio.h>
#include <stdlib.h>
#include "err.h"
#include "print.h"

// Главное меню
void print_menu(void)
{
    printf("\n");
    printf("    +-----------------------------------------------------------+\n");
    printf("    |                     Меню операций с матрицами             |\n");
    printf("    +-----------------------------------------------------------+\n");
    printf("    | 0 - Выход.                                                |\n");
    printf("    | 1 - Ввод CSR матрицы.                                     |\n");
    printf("    | 2 - Ввод CSC матрицы.                                     |\n");
    printf("    | 3 - Ввод стандартных матриц.                              |\n");
    printf("    | 4 - Вывод CSR матрицы.                                    |\n");
    printf("    | 5 - Вывод CSC матрицы.                                    |\n");
    printf("    | 6 - Вывод CSR матрицы в стандартном виде.                 |\n");
    printf("    | 7 - Вывод CSC матрицы в стандартном виде.                 |\n");
    printf("    | 8 - Вывод стандартных матриц.                             |\n");
    printf("    | 9 - Умножение разреженных матриц (CSR x CSC).             |\n");
    printf("    | 10 - Умножение стандартных матриц.                        |\n");
    printf("    | 11 - Сравнение производительности.                        |\n");
    printf("    | 12 - Создание матрицы.                                    |\n");
    printf("    +-----------------------------------------------------------+\n");
    printf("Выбор: ");
}

// Вывод результата
void print_error(int rc)
{
    switch (rc)
    {
        case ERROR_FILE:
            printf("Ошибка работы с файлом!!!\n");
            break;
        case ERROR_INPUT:
            printf("Ошибка ввода!!!\n");
            break;
        case ERROR_IO:
            printf("Ошибка чтения содержания файла!!!\n");
            break;
        case ERROR_MEM:
            printf("Ошибка памяти!!!\n");
            break;
        case ERROR_SIZE:
            printf("Ошибка невалидных размеров матриц!!!\n");
            break;
        case ERROR_EMPTY_FILE:
            printf("Ошибка пустого файла!!!\n");
            break;
    }
}