#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "err.h"
#include "memory.h"

// Функция для инициализации CSR матрицы нулями
void init_csr_matrix(CSRMatrix* matrix)
{
    matrix->A = NULL;
    matrix->IA = NULL;
    matrix->JA = NULL;
    matrix->lines = 0;
    matrix->columns = 0;
    matrix->num_elem = 0;
}

// Функция для инициализации CSC матрицы нулями
void init_csc_matrix(CSCMatrix* matrix)
{
    matrix->B = NULL;
    matrix->JB = NULL;
    matrix->IB = NULL;
    matrix->lines = 0;
    matrix->columns = 0;
    matrix->num_elem = 0;
}

// Создание матрицы
double** allocate_matrix(size_t lines, size_t columns)
{
    double** matrix = malloc(lines * sizeof(double*));
    if (matrix == NULL)
        return NULL;
    
    for (size_t i = 0; i < lines; i++)
    {
        matrix[i] = calloc(columns, sizeof(double));
        if (matrix[i] == NULL)
        {
            free_matrix(matrix, i);
            return NULL;
        }
    }
    return matrix;
}

// Освобождение матрицы
void free_matrix(double** matrix, size_t lines)
{
    if (matrix == NULL)
        return;

    for (size_t i = 0; i < lines; i++)
    {
        if (matrix[i] != NULL)
            free(matrix[i]);
    }
    free(matrix);
}

// Выделение памяти под CSR матрицу
int allocate_csr_matrix(CSRMatrix* matrix, size_t lines, size_t columns, size_t num_elem)
{
    matrix->lines = lines;
    matrix->columns = columns;
    matrix->num_elem = num_elem;
    matrix->A = malloc(num_elem * sizeof(double));
    matrix->IA = malloc(num_elem * sizeof(int));
    matrix->JA = malloc((lines + 1) * sizeof(int));
    
    if (!matrix->A || !matrix->IA || !matrix->JA)
    {
        if (matrix->A)
            free(matrix->A);
        if (matrix->IA)
            free(matrix->IA);
        if (matrix->JA)
            free(matrix->JA);
        return ERROR_MEM;
    }

    for (size_t i = 0; i <= lines; i++)
        matrix->JA[i] = 0;
    
    return ERROR_OK;
}

// Освобождение CSR матрицы
void free_csr_matrix(CSRMatrix* matrix)
{
    if (matrix->A)
        free(matrix->A);
    if (matrix->IA)
        free(matrix->IA);
    if (matrix->JA)
        free(matrix->JA);
    matrix->A = NULL;
    matrix->IA = NULL;
    matrix->JA = NULL;
}

// Создание CSC матрицы
int allocate_csc_matrix(CSCMatrix* matrix, size_t lines, size_t columns, size_t num_elem)
{
    matrix->lines = lines;
    matrix->columns = columns;
    matrix->num_elem = num_elem;
    matrix->B = malloc(num_elem * sizeof(double));
    matrix->JB = malloc(num_elem * sizeof(int));
    matrix->IB = malloc((columns + 1) * sizeof(int));
    
    if (!matrix->B || !matrix->JB || !matrix->IB)
    {
        if (matrix->B)
            free(matrix->B);
        if (matrix->JB)
            free(matrix->JB);
        if (matrix->IB)
            free(matrix->IB);
        return ERROR_MEM;
    }

    for (size_t i = 0; i <= columns; i++)
        matrix->IB[i] = 0;
    
    return ERROR_OK;
}

// Освобождение CSC матрицы
void free_csc_matrix(CSCMatrix* matrix)
{
    if (matrix->B)
        free(matrix->B);
    if (matrix->JB)
        free(matrix->JB);
    if (matrix->IB)
        free(matrix->IB);
    matrix->B = NULL;
    matrix->JB = NULL;
    matrix->IB = NULL;
}