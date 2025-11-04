#ifndef STRUCT_H__
#define STRUCT_H__

#include <stdio.h>

// Структура для разреженной матрицы в формате CSR
typedef struct 
{
    double* A;       // значения ненулевых элементов
    int* IA;         // номера столбцов для элементов A
    int* JA;         // индексы начала строк в A и IA
    size_t num_elem; // количество ненулевых элементов
    size_t lines;    // количество строк
    size_t columns;  // количество столбцов
} CSRMatrix;

// Структура для разреженной матрицы в формате CSC
typedef struct 
{
    double* B;       // значения ненулевых элементов
    int* JB;         // номера строк для элементов B
    int* IB;         // индексы начала столбцов в B и JB
    size_t num_elem; // количество ненулевых элементов
    size_t lines;    // количество строк
    size_t columns;  // количество столбцов
} CSCMatrix;

#endif