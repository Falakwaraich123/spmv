// my_csc.c
#include "spmv.h"
#include <stdlib.h>

typedef struct {
    double *values;  // Non-zero values
    int *rowIndex;   // Row indices of non-zero values
    int *colPtr;     // Column pointers
    int nnz;         // Number of non-zero elements
    int ncols;       // Number of columns
} CSCMatrix;

CSCMatrix* convert_to_csc(const double *dense_matrix, int nrows, int ncols) {
    int nnz = 0;
    for (int i = 0; i < nrows * ncols; i++) {
        if (dense_matrix[i] != 0.0) {
            nnz++;
        }
    }

    CSCMatrix *csc = (CSCMatrix *) malloc(sizeof(CSCMatrix));
    csc->values = (double *) malloc(nnz * sizeof(double));
    csc->rowIndex = (int *) malloc(nnz * sizeof(int));
    csc->colPtr = (int *) malloc((ncols + 1) * sizeof(int));
    csc->nnz = nnz;
    csc->ncols = ncols;

    int idx = 0;
    for (int j = 0; j < ncols; j++) {
        csc->colPtr[j] = idx;
        for (int i = 0; i < nrows; i++) {
            double value = dense_matrix[i * ncols + j];
            if (value != 0.0) {
                csc->values[idx] = value;
                csc->rowIndex[idx] = i;
                idx++;
            }
        }
    }
    csc->colPtr[ncols] = nnz;

    return csc;
}

void csc_spmv(const CSCMatrix *csc, const double *vec, double *result) {
    for (int j = 0; j < csc->ncols; j++) {
        for (int i = csc->colPtr[j]; i < csc->colPtr[j + 1]; i++) {
            result[csc->rowIndex[i]] += csc->values[i] * vec[j];
        }
    }
}

int my_csc(const double *dense_matrix, const double *vec, double *result, int size) {
    CSCMatrix *csc_matrix = convert_to_csc(dense_matrix, size, size);
    for (int i = 0; i < size; i++) {
        result[i] = 0.0;
    }

    csc_spmv(csc_matrix, vec, result);

    free(csc_matrix->values);
    free(csc_matrix->rowIndex);
    free(csc_matrix->colPtr);
    free(csc_matrix);

    return 0;
}
