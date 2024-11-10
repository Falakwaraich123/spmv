// my_coo.c
#include "spmv.h"
#include <stdlib.h>

typedef struct {
    double *values;  // Non-zero values
    int *rowIndex;   // Row indices of non-zero values
    int *colIndex;   // Column indices of non-zero values
    int nnz;         // Number of non-zero elements
} COOMatrix;

COOMatrix* convert_to_coo(const double *dense_matrix, int nrows, int ncols) {
    int nnz = 0;
    for (int i = 0; i < nrows * ncols; i++) {
        if (dense_matrix[i] != 0.0) {
            nnz++;
        }
    }

    COOMatrix *coo = (COOMatrix *) malloc(sizeof(COOMatrix));
    coo->values = (double *) malloc(nnz * sizeof(double));
    coo->rowIndex = (int *) malloc(nnz * sizeof(int));
    coo->colIndex = (int *) malloc(nnz * sizeof(int));
    coo->nnz = nnz;

    int idx = 0;
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            double value = dense_matrix[i * ncols + j];
            if (value != 0.0) {
                coo->values[idx] = value;
                coo->rowIndex[idx] = i;
                coo->colIndex[idx] = j;
                idx++;
            }
        }
    }

    return coo;
}

void coo_spmv(const COOMatrix *coo, const double *vec, double *result) {
    for (int i = 0; i < coo->nnz; i++) {
        result[coo->rowIndex[i]] += coo->values[i] * vec[coo->colIndex[i]];
    }
}

int my_coo(const double *dense_matrix, const double *vec, double *result, int size) {
    COOMatrix *coo_matrix = convert_to_coo(dense_matrix, size, size);
    for (int i = 0; i < size; i++) {
        result[i] = 0.0;
    }

    coo_spmv(coo_matrix, vec, result);

    free(coo_matrix->values);
    free(coo_matrix->rowIndex);
    free(coo_matrix->colIndex);
    free(coo_matrix);

    return 0;
}
