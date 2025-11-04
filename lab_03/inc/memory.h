#ifndef MEMORY_H__
#define MEMORY_H__

// Функция для инициализации CSR матрицы нулями
void init_csr_matrix(CSRMatrix* matrix);

// Функция для инициализации CSC матрицы нулями
void init_csc_matrix(CSCMatrix* matrix);

// Создание матрицы
double** allocate_matrix(size_t lines, size_t columns);

// Освобождение матрицы
void free_matrix(double** matrix, size_t lines);

// Выделение памяти под CSR матрицу
int allocate_csr_matrix(CSRMatrix* matrix, size_t lines, size_t columns, size_t num_elem);

// Освобождение CSR матрицы
void free_csr_matrix(CSRMatrix* matrix);

// Создание CSC матрицы
int allocate_csc_matrix(CSCMatrix* matrix, size_t lines, size_t columns, size_t num_elem);

// Освобождение CSC матрицы
void free_csc_matrix(CSCMatrix* matrix);

#endif