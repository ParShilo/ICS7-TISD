#ifndef IO_H__
#define IO_H__

// Ввод CSR матрицы с клавиатуры
int input_csr_matrix(CSRMatrix* matrix);

// Загрузка CSR матрицы из файла
int load_csr_matrix_from_file(CSRMatrix* matrix);

// Вывод CSR матрицы
void print_csr_matrix(const CSRMatrix* matrix);

// Вывод CSR матрицы в виде обычной матрицы
void print_csr_normal(const CSRMatrix* matrix);

// Ввод CSC матрицы с клавиатуры
int input_csc_matrix(CSCMatrix* matrix);

// Загрузка CSC матрицы из файла
int load_csc_matrix_from_file(CSCMatrix* matrix);

// Вывод CSC матрицы
void print_csc_matrix(const CSCMatrix* matrix);

// Вывод CSC матрицы в виде обычной матрицы
void print_csc_normal(const CSCMatrix* matrix);

// Вывод стандартной матрицы
void print_matrix(double** matrix, size_t lines, size_t columns);

// Ввод стандартной матрицы
int input_matrix(double*** matrix, size_t* lines, size_t* columns);

// Загрузка стандартной матрицы из файла
int load_matrix_from_file(double*** matrix, size_t* lines, size_t* columns);

#endif