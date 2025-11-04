#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "err.h"
#include "memory.h"
#include "io.h"

// Ввод CSR матрицы с клавиатуры
int input_csr_matrix(CSRMatrix* matrix)
{
    int tmp, rc = ERROR_OK;

    printf("Введите количество строк матрицы: ");
    if (scanf("%d", &tmp) != 1 || tmp < 1)
        return ERROR_INPUT;
    matrix->lines = tmp;
    
    printf("Введите количество столбцов матрицы: ");
    if (scanf("%d", &tmp) != 1 || tmp < 1)
        return ERROR_INPUT;
    matrix->columns = tmp;

    // Освобождение предыдущей части
    free_csr_matrix(matrix);

    printf("Введите количество ненулевых элементов: ");
    if (scanf("%d", &tmp) != 1 || tmp < 0 || tmp > (int)(matrix->lines * matrix->columns))
        return ERROR_INPUT;
    matrix->num_elem = tmp;

    if (matrix->num_elem > matrix->lines * matrix->columns)
    {
        printf("Ошибка: слишком много ненулевых элементов!\n");
        return ERROR_INPUT;
    }

    // Выделение памяти
    rc = allocate_csr_matrix(matrix, matrix->lines, matrix->columns, matrix->num_elem);
    if (rc != ERROR_OK)
        return rc;

    // Инициализация JA нулями
    for (size_t i = 0; i <= matrix->lines; i++)
        matrix->JA[i] = 0;

    printf("Введите элементы в формате (строка, столбец, значение):\n");
    size_t current_row = 0;
    size_t elem_count = 0;
    size_t row, col;
    double value;
    int tmp_1, tmp_2;

    for (size_t i = 0; i < matrix->num_elem; i++)
    {
        if (scanf("%d %d %lf", &tmp_1, &tmp_2, &value) != 3)
            return ERROR_INPUT;
        row = tmp_1, col = tmp_2;

        if (row >= matrix->lines || col >= matrix->columns)
        {
            printf("Ошибка: индексы выходят за границы матрицы!\n");
            return ERROR_INPUT;
        }

        matrix->A[elem_count] = value;
        matrix->IA[elem_count] = col;
        elem_count++;

        // Обновление указателей строк
        while (current_row <= row)
        {
            matrix->JA[current_row] = elem_count - 1;
            current_row++;
        }
    }

    // Завершение указателей строк
    while (current_row <= matrix->lines)
    {
        matrix->JA[current_row] = elem_count;
        current_row++;
    }

    printf("CSR матрица успешно введена!\n");
    return ERROR_OK;
}

// Загрузка CSR матрицы из файла
int load_csr_matrix_from_file(CSRMatrix* matrix)
{
    // Ввод файла
    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return ERROR_FILE;

    free_csr_matrix(matrix);

    // Валидация входных величин
    int tmp_1, tmp_2, tmp_3;
    if (fscanf(file, "%d", &tmp_1) != 1 || tmp_1 < 1)
    {
        if (feof(file))
        {
            fclose(file);
            return ERROR_EMPTY_FILE;
        }
        else
        {
            fclose(file);
            return ERROR_IO;
        }
    }
    if (fscanf(file, "%d %d", &tmp_2, &tmp_3) != 2 || tmp_2 < 1 || tmp_3 < 0 || tmp_3 > tmp_1 * tmp_2)
    {
        fclose(file);
        return ERROR_IO;
    }
    matrix->lines = tmp_1, matrix->columns = tmp_2, matrix->num_elem = tmp_3;

    // Выделение памяти
    int rc = allocate_csr_matrix(matrix, matrix->lines, matrix->columns, matrix->num_elem);
    if (rc != ERROR_OK)
        return rc;

    // Чтение данных
    for (size_t i = 0; i < matrix->num_elem; i++)
    {
        if (fscanf(file, "%lf", &matrix->A[i]) != 1)
        {
            fclose(file);
            return ERROR_IO;
        }
    }

    for (size_t i = 0; i < matrix->num_elem; i++)
    {
        if (fscanf(file, "%d", &matrix->IA[i]) != 1)
        {
            fclose(file);
            return ERROR_IO;
        }
    }

    for (size_t i = 0; i <= matrix->lines; i++)
    {
        if (fscanf(file, "%d", &matrix->JA[i]) != 1)
        {
            fclose(file);
            return ERROR_IO;
        }
    }

    fclose(file);
    printf("CSR матрица успешно загружена из файла!\n");
    return ERROR_OK;
}

// Вывод CSR матрицы
void print_csr_matrix(const CSRMatrix* matrix)
{
    printf("CSR матрица (%zux%zu), ненулевых элементов: %zu\n", matrix->lines, matrix->columns, matrix->num_elem);
    printf("A (значения): ");
    for (size_t i = 0; i < matrix->num_elem; i++)
        printf("%.2f ", matrix->A[i]);
    printf("\nIA (столбцы): ");
    for (size_t i = 0; i < matrix->num_elem; i++)
        printf("%d ", matrix->IA[i]);
    printf("\nJA (указатели строк): ");
    for (size_t i = 0; i <= matrix->lines; i++)
        printf("%d ", matrix->JA[i]);
    printf("\n");
}

// Вывод CSR матрицы в виде обычной матрицы
void print_csr_normal(const CSRMatrix* matrix)
{
    printf("CSR матрица в стандартном виде (%zux%zu):\n", matrix->lines, matrix->columns);
    
    size_t row_start, row_end;
    double value;

    for (size_t i = 0; i < matrix->lines; i++)
    {        
        // Начало и конец текущей строки в векторах A и IA
        row_start = matrix->JA[i];
        row_end = matrix->JA[i + 1];
        
        // Цикл по всем столбцам текущей строки
        for (size_t j = 0; j < matrix->columns; j++)
        {
            value = 0.0;
            
            // Поиск элемента в текущем столбце j
            for (size_t k = row_start; k < row_end; k++)
            {
                if (matrix->IA[k] == (int)j)
                {
                    value = matrix->A[k];
                    break;
                }
            }
            
            printf("%8.2f ", value);
        }
        printf("\n");
    }
}

// Ввод CSC матрицы с клавиатуры
int input_csc_matrix(CSCMatrix* matrix)
{
    int tmp, rc = ERROR_OK;

    printf("Введите количество строк матрицы: ");
    if (scanf("%d", &tmp) != 1 || tmp < 1)
        return ERROR_INPUT;
    matrix->lines = tmp;
    
    printf("Введите количество столбцов матрицы: ");
    if (scanf("%d", &tmp) != 1 || tmp < 1)
        return ERROR_INPUT;
    matrix->columns = tmp;

    // Освобождение предыдущей памяти
    free_csc_matrix(matrix);

    printf("Введите количество ненулевых элементов: ");
    if (scanf("%d", &tmp) != 1 || tmp < 0)
        return ERROR_INPUT;
    matrix->num_elem = tmp;

    if (matrix->num_elem > matrix->lines * matrix->columns)
    {
        printf("Ошибка: слишком много ненулевых элементов!\n");
        return ERROR_INPUT;
    }

    // Выделение памяти
    rc = allocate_csc_matrix(matrix, matrix->lines, matrix->columns, matrix->num_elem);
    if (rc != ERROR_OK)
        return rc;

    // Инициализация IB нулями
    for (size_t i = 0; i <= matrix->columns; i++)
        matrix->IB[i] = 0;

    printf("Введите элементы в формате (строка, столбец, значение):\n");
    size_t current_col = 0;
    size_t elem_count = 0;
    size_t row, col;
    double value;
    int tmp_1, tmp_2;

    for (size_t i = 0; i < matrix->num_elem; i++)
    {
        if (scanf("%d %d %lf", &tmp_1, &tmp_2, &value) != 3)
            return ERROR_INPUT;
        row = tmp_1, col = tmp_2;

        if (row >= matrix->lines || col >= matrix->columns)
        {
            printf("Ошибка: индексы выходят за границы матрицы!\n");
            return ERROR_INPUT;
        }

        matrix->B[elem_count] = value;
        matrix->JB[elem_count] = row;
        elem_count++;

        // Обновление указателей столбцов
        while (current_col <= col)
        {
            matrix->IB[current_col] = elem_count - 1;
            current_col++;
        }
    }

    // Завершение указателей столбцов
    while (current_col <= matrix->columns)
    {
        matrix->IB[current_col] = elem_count;
        current_col++;
    }

    printf("CSC матрица успешно введена!\n");
    return ERROR_OK;
}

// Загрузка CSC матрицы из файла
int load_csc_matrix_from_file(CSCMatrix* matrix)
{
    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return ERROR_FILE;

    free_csc_matrix(matrix);

    // Валидация входных величин
    int tmp_1, tmp_2, tmp_3;
    if (fscanf(file, "%d", &tmp_1) != 1 || tmp_1 < 1)
    {
        if (feof(file))
        {
            fclose(file);
            return ERROR_EMPTY_FILE;
        }
        else
        {
            fclose(file);
            return ERROR_IO;
        }
    }
    if (fscanf(file, "%d %d", &tmp_2, &tmp_3) != 2 || tmp_2 < 1 || tmp_3 < 0 || tmp_3 > tmp_1 * tmp_2)
    {
        fclose(file);
        return ERROR_IO;
    }
    matrix->lines = tmp_1, matrix->columns = tmp_2, matrix->num_elem = tmp_3;

    // Выделение памяти
    int rc = allocate_csc_matrix(matrix, matrix->lines, matrix->columns, matrix->num_elem);
    if (rc != ERROR_OK)
        return rc;

    // Чтение данных
    for (size_t i = 0; i < matrix->num_elem; i++)
    {
        if (fscanf(file, "%lf", &matrix->B[i]) != 1)
        {
            fclose(file);
            return ERROR_IO;
        }
    }

    for (size_t i = 0; i < matrix->num_elem; i++)
    {
        if (fscanf(file, "%d", &matrix->JB[i]) != 1)
        {
            fclose(file);
            return ERROR_IO;
        }
    }

    for (size_t i = 0; i <= matrix->columns; i++)
    {
        if (fscanf(file, "%d", &matrix->IB[i]) != 1)
        {
            fclose(file);
            return ERROR_IO;
        }
    }

    fclose(file);
    printf("CSC матрица успешно загружена из файла!\n");
    return ERROR_OK;
}

// Вывод CSC матрицы
void print_csc_matrix(const CSCMatrix* matrix)
{
    printf("CSC матрица (%zux%zu), ненулевых элементов: %zu\n", matrix->lines, matrix->columns, matrix->num_elem);
    printf("B (значения): ");
    for (size_t i = 0; i < matrix->num_elem; i++)
        printf("%.2f ", matrix->B[i]);
    printf("\nJB (строки): ");
    for (size_t i = 0; i < matrix->num_elem; i++)
        printf("%d ", matrix->JB[i]);
    printf("\nIB (указатели столбцов): ");
    for (size_t i = 0; i <= matrix->columns; i++)
        printf("%d ", matrix->IB[i]);
    printf("\n");
}

// Вывод CSC матрицы в виде обычной матрицы
void print_csc_normal(const CSCMatrix* matrix)
{
    printf("CSC матрица в стандартном виде (%zux%zu):\n", matrix->lines, matrix->columns);
    
    size_t col_start, col_end;
    double value;

    for (size_t i = 0; i < matrix->lines; i++)
    {        
        // Цикл по всем столбцам текущей строки
        for (size_t j = 0; j < matrix->columns; j++)
        {
            value = 0.0;
            
            // Начало и конец текущего столбца в векторах B и JB
            col_start = matrix->IB[j];
            col_end = matrix->IB[j + 1];
            
            // Поиск элемента в текущей строке i и столбце j
            for (size_t k = col_start; k < col_end; k++)
            {
                if (matrix->JB[k] == (int)i)
                {
                    value = matrix->B[k];
                    break;
                }
            }
            
            printf("%8.2f ", value);
        }
        printf("\n");
    }
}

// Вывод стандартной матрицы
void print_matrix(double** matrix, size_t lines, size_t columns)
{
    printf("Стандартная матрица (%zux%zu):\n", lines, columns);
    for (size_t i = 0; i < lines; i++)
    {
        for (size_t j = 0; j < columns; j++)
            printf("%8.2f ", matrix[i][j]);
        printf("\n");
    }
}

// Ввод стандартной матрицы
int input_matrix(double*** matrix, size_t* lines, size_t* columns)
{
    int tmp;
    
    printf("Введите количество строк матрицы: ");
    if (scanf("%d", &tmp) != 1 || tmp < 1)
        return ERROR_IO;
    *lines = tmp;

    printf("Введите количество столбцов матрицы: ");
    if (scanf("%d", &tmp) != 1 || tmp < 1)
        return ERROR_IO;
    *columns = tmp;

    *matrix = allocate_matrix(*lines, *columns);
    if (*matrix == NULL)
        return ERROR_MEM;

    printf("Введите элементы матрицы (%zux%zu):\n", *lines, *columns);
    for (size_t i = 0; i < *lines; i++)
    {
        for (size_t j = 0; j < *columns; j++)
        {
            printf("Элемент [%zu][%zu]: ", i, j);
            if (scanf("%lf", &(*matrix)[i][j]) != 1)
            {
                free_matrix(*matrix, *lines);
                return ERROR_IO;
            }
        }
    }

    printf("Стандартная матрица успешно введена!\n");
    return ERROR_OK;
}

// Загрузка стандартной матрицы из файла
int load_matrix_from_file(double*** matrix, size_t* lines, size_t* columns)
{
    int tmp_1, tmp_2;
    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка открытия файла '%s'\n", filename);
        return ERROR_FILE;
    }

    // Валидация входных величин
    if (fscanf(file, "%d", &tmp_1) != 1 || tmp_1 < 1)
    {
        if (feof(file))
        {
            fclose(file);
            return ERROR_EMPTY_FILE;
        }
        else
        {
            fclose(file);
            return ERROR_IO;
        }
    }
    if (fscanf(file, "%d", &tmp_2) != 1 || tmp_2 < 1)
    {
        fclose(file);
        return ERROR_IO;
    }
    *lines = tmp_1, *columns = tmp_2;

    *matrix = allocate_matrix(*lines, *columns);
    if (*matrix == NULL)
    {
        fclose(file);
        return ERROR_MEM;
    }

    for (size_t i = 0; i < *lines; i++)
    {
        for (size_t j = 0; j < *columns; j++)
        {
            if (fscanf(file, "%lf", &(*matrix)[i][j]) != 1)
            {
                free_matrix(*matrix, *lines);
                fclose(file);
                return ERROR_IO;
            }
        }
    }

    fclose(file);
    return ERROR_OK;
}