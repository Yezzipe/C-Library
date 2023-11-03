#pragma once
#include <stdlib.h>
typedef struct matrix {
    int rows;
    int cols;
    size_t value_size;
    void* values;
} matrix_t;

typedef enum { TXT, BIN } file_mode_t;

matrix_t* consMatrix(int rows, int cols, size_t value_size);
matrix_t* cpyMatrix(matrix_t* M);
void freeMatrix(matrix_t** ptrM);
matrix_t* addMatrix(matrix_t* A, matrix_t* B);
matrix_t* multMatrix(matrix_t* A, matrix_t* B);
matrix_t* readMatrix(char* filename, file_mode_t mode);
void saveMatrix(matrix_t* M, char* filename, file_mode_t mode);
void setMatrix(matrix_t* M, int a, int b, void* value);