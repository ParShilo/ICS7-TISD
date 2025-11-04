#ifndef PROCESS_H__
#define PROCESS_H__

// Умножение разреженных матриц CSR × CSC
int multiply_sparse_csr_csc(const CSRMatrix *A, const CSCMatrix *B, CSRMatrix *result);

// Стандартное умножение плотных матриц
double** multiply_matrices(double** A, size_t A_lines, size_t A_columns, double** B, size_t B_lines, size_t B_columns);

// Преобразование стандартной матрицы в CSR
CSRMatrix normal_to_csr(double** normal, size_t lines, size_t columns);

// Преобразование стандартной матрицы в CSC
CSCMatrix normal_to_csc(double** normal, size_t lines, size_t columns);

// Сравнение производительности
void compare_performance(double** A, size_t A_lines, size_t A_columns, double** B, size_t B_lines, size_t B_columns);

// Генерация стандартной матрицы
int generate_matrix(void);

#endif
