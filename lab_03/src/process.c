#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "struct.h"
#include "err.h"
#include "memory.h"
#include "io.h"
#include "process.h"

// Умножение разреженных матриц CSR × CSC
int multiply_sparse_csr_csc(const CSRMatrix *A, const CSCMatrix *B, CSRMatrix *result)
{
    if (A->columns != B->lines)
        return ERROR_SIZE;

    // Инициализация результирующей матрицы
    init_csr_matrix(result);
    result->lines = A->lines;
    result->columns = B->columns;
    result->num_elem = 0;

    // Максимальное возможное количество ненулевых элементов
    size_t max_num_elem = A->lines * B->columns;

    // Выделение памяти под результирующую матрицу
    int rc = allocate_csr_matrix(result, result->lines, result->columns, max_num_elem);
    if (rc != ERROR_OK)
        return rc;

    // Временный массив для хранения строки результата
    double *temp_row = calloc(B->columns, sizeof(double));
    if (temp_row == NULL)
        return ERROR_MEM;

    result->JA[0] = 0;
    size_t result_idx = 0, a_row_start, a_row_end, k, b_col_start, b_col_end;
    double val_A;

    // Умножение матриц: для каждой строки A
    for (size_t i = 0; i < A->lines; i++)
    {
        // Очищение временной строки
        memset(temp_row, 0, B->columns * sizeof(double));
        
        // Получение диапазона элементов строки i матрицы A
        a_row_start = A->JA[i];
        a_row_end = A->JA[i + 1];
        
        // Цикл для каждого ненулевого элемента в строке A
        for (size_t a_idx = a_row_start; a_idx < a_row_end; a_idx++)
        {
            k = A->IA[a_idx];    // Столбец в A
            val_A = A->A[a_idx]; // Значение A[i][k]
            
            // Поиск элементов строки k матрицы B во всех столбцах
            for (size_t j = 0; j < B->columns; j++)
            {
                // Поиск элемента B[k][j] в столбце j
                b_col_start = B->IB[j];
                b_col_end = B->IB[j + 1];
                
                for (size_t b_idx = b_col_start; b_idx < b_col_end; b_idx++)
                {
                    if (B->JB[b_idx] == (int)k)
                    {
                        temp_row[j] += val_A * B->B[b_idx];
                        break;
                    }
                }
            }
        }
        
        // Перенос ненулевых элементов в результат
        for (size_t j = 0; j < B->columns; j++)
        {
            if (fabs(temp_row[j]) > 1e-12)
            {
                if (result_idx >= max_num_elem)
                {
                    free(temp_row);
                    return ERROR_MEM;
                }
                
                result->A[result_idx] = temp_row[j];
                result->IA[result_idx] = j;
                result_idx++;
            }
        }
        
        result->JA[i + 1] = result_idx;
    }

    // Обновление фактического количества ненулевых элементов
    result->num_elem = result_idx;
    
    free(temp_row);
    return ERROR_OK;
}

// Стандартное умножение матриц
double** multiply_matrices(double** A, size_t A_lines, size_t A_columns, double** B, size_t B_lines, size_t B_columns)
{
    if (A_columns != B_lines)
        return NULL;
    
    double** result = allocate_matrix(A_lines, B_columns);
    if (result == NULL)
        return NULL;
    
    for (size_t i = 0; i < A_lines; i++)
    {
        for (size_t j = 0; j < B_columns; j++)
        {
            result[i][j] = 0;
            for (size_t k = 0; k < A_columns; k++)
                result[i][j] += A[i][k] * B[k][j];
        }
    }
    
    return result;
}

// Преобразование стандартной матрицы в CSR
CSRMatrix normal_to_csr(double** normal, size_t lines, size_t columns)
{
    size_t num_elem = 0;
    
    // Подсчет количества ненулевых элементов
    for (size_t i = 0; i < lines; i++)
        for (size_t j = 0; j < columns; j++)
            if (fabs(normal[i][j]) > 1e-12)
                num_elem++;
    
    CSRMatrix csr;
    csr.lines = lines;
    csr.columns = columns;
    csr.num_elem = num_elem;
    
    // Выделение памяти
    int rc = allocate_csr_matrix(&csr, lines, columns, num_elem);
    if (rc != ERROR_OK)
    {
        printf("Ошибка выделения памяти для CSR матрицы!\n");
        csr.A = NULL;
        csr.IA = NULL;
        csr.JA = NULL;
        return csr;
    }
    
    size_t idx = 0;
    csr.JA[0] = 0;
    
    // Заполнение CSR векторов
    for (size_t i = 0; i < lines; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            if (fabs(normal[i][j]) > 1e-12)
            {
                csr.A[idx] = normal[i][j];
                csr.IA[idx] = j;
                idx++;
            }
        }
        csr.JA[i + 1] = idx;
    }
    
    return csr;
}

// Преобразование стандартной матрицы в CSC
CSCMatrix normal_to_csc(double** normal, size_t lines, size_t columns)
{
    size_t num_elem = 0;
    
    // Подсчет количества ненулевых элементов
    for (size_t j = 0; j < columns; j++)
        for (size_t i = 0; i < lines; i++)
            if (fabs(normal[i][j]) > 1e-12)
                num_elem++;
    
    CSCMatrix csc;
    csc.lines = lines;
    csc.columns = columns;
    csc.num_elem = num_elem;
    
    // Выделение памяти
    int rc = allocate_csc_matrix(&csc, lines, columns, num_elem);
    if (rc != ERROR_OK)
    {
        printf("Ошибка выделения памяти для CSC матрицы!\n");
        csc.B = NULL;
        csc.JB = NULL;
        csc.IB = NULL;
        return csc;
    }
    
    size_t idx = 0;
    csc.IB[0] = 0;
    
    // Заполнение CSC векторов
    for (size_t j = 0; j < columns; j++)
    {
        for (size_t i = 0; i < lines; i++)
        {
            if (fabs(normal[i][j]) > 1e-12)
            {
                csc.B[idx] = normal[i][j];
                csc.JB[idx] = i;
                idx++;
            }
        }
        csc.IB[j + 1] = idx;
    }
    
    return csc;
}

// Преобразование CSR в стандартную матрицу
double** csr_to_normal(const CSRMatrix* csr)
{
    double** normal = allocate_matrix(csr->lines, csr->columns);
    if (!normal)
        return NULL;
    
    // Инициализация нулями
    for (size_t i = 0; i < csr->lines; i++)
        for (size_t j = 0; j < csr->columns; j++)
            normal[i][j] = 0.0;
    
    // Заполнение ненулевыми элементами
    for (size_t i = 0; i < csr->lines; i++)
    {
        size_t row_start = csr->JA[i];
        size_t row_end = csr->JA[i + 1];
        
        for (size_t j = row_start; j < row_end; j++)
        {
            size_t col = csr->IA[j];
            normal[i][col] = csr->A[j];
        }
    }
    
    return normal;
}

// Преобразование CSC в стандартную матрицу
double** csc_to_normal(const CSCMatrix* csc)
{
    double** normal = allocate_matrix(csc->lines, csc->columns);
    if (!normal)
        return NULL;
    
    // Инициализация нулями
    for (size_t i = 0; i < csc->lines; i++)
        for (size_t j = 0; j < csc->columns; j++)
            normal[i][j] = 0.0;
    
    // Заполнение ненулевыми элементами
    for (size_t j = 0; j < csc->columns; j++)
    {
        size_t col_start = csc->IB[j];
        size_t col_end = csc->IB[j + 1];
        
        for (size_t i = col_start; i < col_end; i++)
        {
            size_t row = csc->JB[i];
            normal[row][j] = csc->B[i];
        }
    }
    
    return normal;
}

// Сравнение производительности
void compare_performance(double** A, size_t A_lines, size_t A_columns, double** B, size_t B_lines, size_t B_columns)
{
    printf("\n=== Сравнение производительности ===\n");
    printf("Матрицы: %zux%zu и %zux%zu\n", A_lines, A_columns, B_lines, B_columns);
    
    // Конвертация в разреженные форматы
    CSRMatrix A_csr = normal_to_csr(A, A_lines, A_columns);
    CSCMatrix B_csc = normal_to_csc(B, B_lines, B_columns);
    CSRMatrix result_sparse;
    
    int rc;
    double** result_normal = NULL;
    const size_t num_runs = 3;
    double avg_time_sparse = 0.0;
    double avg_time_normal = 0.0;
    
    //Измерение времени разреженного умножения
    for (size_t run = 0; run < num_runs; run++)
    {
        clock_t start = clock();
        rc = multiply_sparse_csr_csc(&A_csr, &B_csc, &result_sparse);
        clock_t end = clock();
        
        if (rc != ERROR_OK)
        {
            printf("Ошибка во время сравнения производительности.\n");
            free_csr_matrix(&A_csr);
            free_csc_matrix(&B_csc);
            return;
        }

        avg_time_sparse += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        //print_csr_normal(&result_sparse);
        free_csr_matrix(&result_sparse);
    }
    
    // Измерение времени стандартного умножения
    for (size_t run = 0; run < num_runs; run++)
    {
        clock_t start = clock();
        result_normal = multiply_matrices(A, A_lines, A_columns, B, B_lines, B_columns);
        clock_t end = clock();
        
        avg_time_normal += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        //print_matrix(result_normal, A_lines, B_columns);
        if (result_normal)
            free_matrix(result_normal, A_lines);
    }
    
    // Вычисление средних значений
    avg_time_sparse /= num_runs;
    avg_time_normal /= num_runs;
    
    // Вычисление используемой памяти
    size_t memory_normal = A_lines * A_columns * sizeof(double) + B_lines * B_columns * sizeof(double);
    size_t memory_sparse = (A_csr.num_elem + B_csc.num_elem) * sizeof(double) + (A_csr.num_elem + B_csc.num_elem) * sizeof(int) + (A_lines + 1 + B_columns + 1) * sizeof(int);
    
    printf("Среднее время:\n");
    printf("    Стандартное умножение: %.6f сек\n", avg_time_normal);
    printf("    Разреженное умножение: %.6f сек\n", avg_time_sparse);
    
    if (avg_time_sparse > 0)
        printf("Отношение времени: стандартное/разреженное = %.2f\n", avg_time_normal / avg_time_sparse);
    
    printf("Использование памяти:\n");
    printf("    Стандартные матрицы: %zu байт\n", memory_normal);
    printf("    Разреженные матрицы: %zu байт\n", memory_sparse);

    if (memory_sparse > 0)
        printf("Отношение памяти: стандартное/разреженное = %.2f\n", (double)memory_normal / (double)memory_sparse);
    
    // Освобождение памяти
    free_csr_matrix(&A_csr);
    free_csc_matrix(&B_csc);
}

// Генерация стандартной матрицы
int generate_matrix(void)
{
    char filename[100];
    size_t lines, columns;
    double fill_percentage;
    
    // Ввод параметров
    printf("Введите имя файла для сохранения матрицы: ");
    if (scanf("%99s", filename) != 1)
        return ERROR_INPUT;
    
    printf("Введите количество строк: ");
    if (scanf("%zu", &lines) != 1 || lines < 1)
        return ERROR_INPUT;
    
    printf("Введите количество столбцов: ");
    if (scanf("%zu", &columns) != 1 || columns < 1)
        return ERROR_INPUT;
    
    printf("Введите процент заполнения (0-100): ");
    if (scanf("%lf", &fill_percentage) != 1 || fill_percentage < 0 || fill_percentage > 100)
        return ERROR_INPUT;

    // Открытие файла
    FILE* file = fopen(filename, "w");
    if (file == NULL)
        return ERROR_FILE;

    // Запись размеров матрицы
    fprintf(file, "%zu %zu\n", lines, columns);

    // Инициализация генератора случайных чисел
    srand(time(NULL));
    
    size_t total_elements = lines * columns;
    size_t non_zero_elements = (size_t)(total_elements * fill_percentage / 100.0);
    
    // Создание временного массива для контроля заполнения
    int* filled = calloc(total_elements, sizeof(int));
    if (filled == NULL)
    {
        fclose(file);
        return ERROR_MEM;
    }

    // Заполнение случайных позиций ненулевыми значениями
    for (size_t i = 0; i < non_zero_elements; i++)
    {
        size_t pos;
        do {
            pos = rand() % total_elements;
        } while (filled[pos]);
        
        filled[pos] = 1;
    }

    double value;

    // Запись матрицы в файл
    for (size_t i = 0; i < lines; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            size_t pos = i * columns + j;
            if (filled[pos])
            {
                value = (rand() % 20001 - 10000) / 100.0;
                fprintf(file, "%.2f", value);
            }
            else
                fprintf(file, "0.00");
            
            if (j < columns - 1)
                fprintf(file, " ");
        }
        fprintf(file, "\n");
    }

    free(filled);
    fclose(file);
    
    printf("Стандартная матрица %zux%zu с заполнением %.1f%% успешно создана в файле '%s'\n", lines, columns, fill_percentage, filename);
    return ERROR_OK;
}