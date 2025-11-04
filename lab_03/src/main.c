#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "err.h"
#include "struct.h"
#include "memory.h"
#include "io.h"
#include "print.h"
#include "process.h"

int main(void)
{
    CSRMatrix csr_matrix, res_matrix;
    CSCMatrix csc_matrix;

    // Инициализация всех структур
    init_csr_matrix(&csr_matrix);
    init_csc_matrix(&csc_matrix);
    init_csr_matrix(&res_matrix);
    
    double **matrix_A = NULL, **matrix_B = NULL, **matrix_C = NULL;
    size_t A_lines = 0, A_columns = 0, B_lines = 0, B_columns = 0, C_lines = 0, C_columns = 0;
    int choice = 1, input_method;
    int rc = ERROR_OK;

    printf("\nПрограмма для умножения разреженных матриц (CSR × CSC)\n");
    
    while (rc == ERROR_OK && choice)
    {
        print_menu();
        if (scanf("%d", &choice) != 1 || choice > 12 || choice < 0)
        {
            rc = ERROR_INPUT;
            break;
        }
        printf("\n");

        switch (choice)
        {
            // Ввод CSR матрицы
            case 1:
            {
                printf("1. Ввести с клавиатуры.\n2. Загрузить из файла.\nВыберите метод: ");
                if (scanf("%d", &input_method) != 1)
                    rc = ERROR_INPUT;
                else if (input_method == 1)
                    rc = input_csr_matrix(&csr_matrix);
                else if (input_method == 2)
                    rc = load_csr_matrix_from_file(&csr_matrix);
                break;
            }
            
            // Ввод CSC матрицы
            case 2:
            {
                printf("1. Ввести с клавиатуры.\n2. Загрузить из файла.\nВыберите метод: ");
                if (scanf("%d", &input_method) != 1)
                    rc = ERROR_INPUT;
                else if (input_method == 1)
                    rc = input_csc_matrix(&csc_matrix);
                else if (input_method == 2)
                    rc = load_csc_matrix_from_file(&csc_matrix);
                break;
            }
            
            // Ввод двух стандартных матриц
            case 3:
            {
                printf("1. Ввести с клавиатуры.\n2. Загрузить из файла.\nВыберите метод: ");
                if (scanf("%d", &input_method) != 1)
                    rc = ERROR_INPUT;
                else if (input_method == 1)
                {
                    printf("Матрица A:\n");

                    free_matrix(matrix_A, A_lines);
                    free_matrix(matrix_B, B_lines);
                    matrix_A = NULL; matrix_B = NULL;

                    rc = input_matrix(&matrix_A, &A_lines, &A_columns);
                    if (rc != ERROR_OK) break;
                    printf("Матрица B:\n");
                    rc = input_matrix(&matrix_B, &B_lines, &B_columns);
                }
                else if (input_method == 2)
                {
                    printf("Матрица A:\n");

                    free_matrix(matrix_A, A_lines);
                    free_matrix(matrix_B, B_lines);
                    matrix_A = NULL; matrix_B = NULL;

                    rc = load_matrix_from_file(&matrix_A, &A_lines, &A_columns);
                    if (rc != ERROR_OK) break;
                    printf("Матрица B:\n");
                    rc = load_matrix_from_file(&matrix_B, &B_lines, &B_columns);
                }
                break;
            }
            
            // Вывод CSR матрицы в её формате
            case 4:
            {
                if (csr_matrix.A == NULL)
                    printf("Сначала введите CSR матрицу!\n");
                else
                    print_csr_matrix(&csr_matrix);
                break;
            }
            
            // Вывод CSC матрицы в её формате
            case 5:
            {
                if (csc_matrix.B == NULL)
                    printf("Сначала введите CSR матрицу!\n");
                else
                    print_csc_matrix(&csc_matrix);
                break;
            }

            // Вывод CSR матрицы в стандартном формате
            case 6:
            {
                if (csr_matrix.A == NULL)
                    printf("Сначала введите CSR матрицу!\n");
                else if (csr_matrix.lines > 30 || csr_matrix.columns > 30)
                    printf("Слишком большая матрицы.\n");
                else
                    print_csr_normal(&csr_matrix);
                break;
            }

            // Вывод CSC матрицы в стандартном формате
            case 7:
            {
                if (csc_matrix.B == NULL)
                    printf("Сначала введите CSC матрицу!\n");
                else if (csc_matrix.lines > 30 || csc_matrix.columns > 30)
                    printf("Слишком большая матрицы.\n");
                else
                    print_csc_normal(&csc_matrix);
                break;
            }
            
            // Вывод стандартных матриц
            case 8:
            {
                if (matrix_A && matrix_B)
                {
                    if (A_lines <= 30 && A_columns <= 30)
                    {
                        printf("Матрица A:\n");
                        print_matrix(matrix_A, A_lines, A_columns);
                    } 
                    else
                        printf("Матрица A слишком большая для вывода (%zux%zu)\n", A_lines, A_columns);
                    
                    if (B_lines <= 30 && B_columns <= 30)
                    {
                        printf("Матрица B:\n");
                        print_matrix(matrix_B, B_lines, B_columns);
                    } 
                    else
                        printf("Матрица B слишком большая для вывода (%zux%zu)\n", B_lines, B_columns);
                }
                else
                    printf("Сначала введите стандартные матрицы!\n");
                break;
            }
            
            // Умножения матриц CSR и CSC
            case 9:
            {
                if (csr_matrix.A == NULL || csc_matrix.B == NULL)
                    printf("Сначала введите CSR и CSC матрицы!\n");
                else if (csr_matrix.columns != csc_matrix.lines)
                    printf("Ошибка: размеры матриц не совпадают для умножения!\n");
                else
                {
                    rc = multiply_sparse_csr_csc(&csr_matrix, &csc_matrix, &res_matrix);
                    if (rc == ERROR_OK)
                    {
                        printf("Результат умножения (CSR формат):\n");
                        print_csr_matrix(&res_matrix);

                        if (res_matrix.lines < 30 && res_matrix.columns < 30)
                            print_csr_normal(&res_matrix);
                    }
                }
                break;
            }
            
            // Умножение стандартных матриц
            case 10:
            {
                if (matrix_A == NULL || matrix_B == NULL)
                    printf("Сначала введите стандартные матрицы!\n");
                else if (A_columns != B_lines)
                    printf("Ошибка: размеры матриц не совпадают для умножения!\n");
                else
                {
                    free_matrix(matrix_C, C_lines);
                    matrix_C = NULL;

                    matrix_C = multiply_matrices(matrix_A, A_lines, A_columns, matrix_B, B_lines, B_columns);
                    C_lines = A_lines, C_columns = B_columns;
                    if (C_lines <= 30 && C_columns <= 30)
                    {
                        printf("Результат умножения стандартных матриц:\n");
                        print_matrix(matrix_C, A_lines, B_columns);
                    }
                }
                break;
            }

            // Сравнение производительности
            case 11:
            {
                if (matrix_A == NULL || matrix_B == NULL)
                    printf("Сначала введите стандартные матрицы!\n");
                else
                    compare_performance(matrix_A, A_lines, A_columns, matrix_B, B_lines, B_columns);
                break;
            }
    
            // Создание матрицы
            case 12:
            {
                rc = generate_matrix();
                break;
            }

            case 0:
                printf("Выход из программы.\n");
                break;
                
            default:
                printf("Неверный ввод, попробуйте снова.\n");
        }
    }

    if (rc != ERROR_OK)
        print_error(rc);

    // Освобождение памяти
    free_csr_matrix(&csr_matrix);
    free_csc_matrix(&csc_matrix);
    free_csr_matrix(&res_matrix);
    free_matrix(matrix_A, A_lines);
    free_matrix(matrix_B, B_lines);
    free_matrix(matrix_C, C_lines);

    return rc;
}