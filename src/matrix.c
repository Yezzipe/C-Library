#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

matrix_t* consMatrix(int rows, int cols, size_t value_size) {
    matrix_t* M = malloc(sizeof(matrix_t));
    M->rows = rows;
    M->cols = cols;
    M->value_size = value_size;
    M->values = calloc(rows*cols, value_size);
    return M;
}

matrix_t* cpyMatrix(matrix_t* M) {
    matrix_t* M2 = consMatrix(M->rows, M->cols, M->value_size);
    int size = M->rows*M->cols*M->value_size;
    char* v2 = M2->values;
    char* v = M->values;
    for (int i = 0; i < size; ++i) {
        v2[i] = v[i];
    }
    return M2;
}

void freeMatrix(matrix_t** M) {
    free((*M)->values);
    free(*M);
    *M = NULL;
}

void setMatrix(matrix_t* M, int a, int b, void* value) {
    if (a >= M->rows || a < 0 || b >= M->cols || b < 0) return;
    char* v = value;
    char* Mv = M->values;
    size_t k = b*M->value_size+a*M->value_size*M->cols;
    for (int i = 0; i < M->value_size; ++i) {
        Mv[k+i] = v[i];
    }
}

matrix_t* addMatrix(matrix_t* A, matrix_t* B) {
    if (A->value_size != B->value_size || A->value_size > sizeof(size_t)) return NULL; //CAN'T ADD
    if (A->cols != B->cols || A->rows != B->rows) return NULL; //SIZE ERROR
    matrix_t* M = consMatrix(A->rows, A->cols, A->value_size);
    int size = A->rows*A->cols*A->value_size;
    char* Mv = M->values;
    char* Av = A->values;
    char* Bv = B->values;
    long long int BufferA = 0;
    long long int BufferB = 0;
    long long int BufferM = 0;
    int m = A->value_size;
    int k = 0;
    for (int i = 0; i < size; ++i) {
        BufferA = BufferA | (Av[i] << ((i%m)<<3));
        BufferB = BufferB | (Bv[i] << ((i%m)<<3));
        if (i%m == m-1) {
            BufferM = BufferA+BufferB;
            char* arr = &BufferM;
            for (int j = 0; j < m; ++j) {
                Mv[k*m+j] = arr[j];
            }
            k++;
            BufferM = 0;
            BufferA = 0;
            BufferB = 0;
        }
    }
    return M;
}

matrix_t* multMatrix(matrix_t* A, matrix_t* B) {
    if (A->value_size != B->value_size || A->value_size > sizeof(size_t)) return NULL; //CAN'T MULTIPLY
    if (A->cols != B->rows) return NULL; //SIZE ERROR
    matrix_t* M = consMatrix(A->rows, B->cols, A->value_size);
    int size = A->rows*A->cols;
    char* Mv = M->values;
    char* Av = A->values;
    char* Bv = B->values;
    long long int BufferA = 0;
    long long int BufferB = 0;
    long long int BufferM = 0;
    int m = A->value_size;
    int o = 0;
    for (int k = 0; k < size; ++k) {
        int i = k / A->rows;
        int j = k % A->rows;
        for (int l = 0; l < A->cols; ++l) {
            for (int n = 0; n < m; ++n) {
                BufferA = BufferA | (Av[(i*A->rows+l)*m+n] << ((n%m)<<3));
                BufferB = BufferB | (Bv[(l*A->rows+j)*m+n] << ((n%m)<<3));
            }
            BufferM = BufferA*BufferB;
            char* arr = &BufferM;
            for (int n = 0; n < m; ++n) {
                Mv[o*m+n] = arr[n];
            }
            o++;
            BufferM = 0;
            BufferA = 0;
            BufferB = 0;
        }
    }
    return M;
}

matrix_t* readMatrix(char* filename, file_mode_t mode) {
    FILE* file = fopen(filename, mode ? "rb" : "rt");
    assert(file);
    int rows;
    int cols;
    size_t size;
    matrix_t* M;
    if (mode) {
        fread(&rows, sizeof(int), 1, file);
        fread(&cols, sizeof(int), 1, file);
        fread(&size, sizeof(size_t), 1, file);
        M = consMatrix(rows, cols, size);
        fread(M->values, size, rows*cols, file);
    } else {
        fscanf(file, "%d %d %llu", &rows, &cols, &size);
        M = consMatrix(rows, cols, size);
        char* Mv =  M->values;
        for (int k = 0; k < size*rows*cols; ++k) {
            fscanf(file, "%hhu", Mv[k]);
        }   
    }
    fclose(file);
    return M;
}

void saveMatrix(matrix_t* M, char* filename, file_mode_t mode) {
    FILE* file = fopen(filename, mode ? "wb" : "wt");
    assert(file);
    if (mode) {
        fwrite(&M->rows, sizeof(int), 1, file);
        fwrite(&M->cols, sizeof(int), 1, file);
        fwrite(&M->value_size, sizeof(size_t), 1, file);
        fwrite(M->values, M->value_size, M->rows*M->cols, file);
    } else {
        fprintf(file, "%d %d %llu\n", M->rows, M->cols, M->value_size);
        int size = M->rows*M->cols*M->value_size;
        int sizeC = M->cols*M->value_size;
        char* Mv = M->values;
        for (int k = 0; k < size; ++k) {
            fprintf(file, "%hhu ", Mv[k]);
            if (k % sizeC == sizeC-1) fprintf(file, "\n");
        }
    }
    fclose(file);
}