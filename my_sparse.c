#include "spmv.h"
#include <stdlib.h>


typedef struct {
    double *values; 
    int *colIndex;   
    int *rowPtr;    
    int nrows;      
    int ncols;      
    int nnz;         
} CSRMatrix;

CSRMatrix* convert_to_csr(const double *dense_matrix, int nrows, int ncols) {
    int nnz = 0;
    for (int i = 0; i < nrows * ncols; i++) {
        if (dense_matrix[i] != 0.0) {
            nnz++;
        }
    }

    CSRMatrix *csr = (CSRMatrix *) malloc(sizeof(CSRMatrix));
    csr->values = (double *) malloc(nnz * sizeof(double));
    csr->colIndex = (int *) malloc(nnz * sizeof(int));
    csr->rowPtr = (int *) malloc((nrows + 1) * sizeof(int));
    csr->nrows = nrows;
    csr->ncols = ncols;
    csr->nnz = nnz;

    int idx = 0;
    for (int i = 0; i < nrows; i++) {
        csr->rowPtr[i] = idx; 
        for (int j = 0; j < ncols; j++) {
            double value = dense_matrix[i * ncols + j];
            if (value != 0.0) {
                csr->values[idx] = value;
                csr->colIndex[idx] = j;
                idx++;
            }
        }
    }
    csr->rowPtr[nrows] = nnz; 

    return csr;
}

void csr_spmv(const CSRMatrix *csr, const double *vec, double *result) {
    for (int i = 0; i < csr->nrows; i++) {
        result[i] = 0.0; 
        for (int j = csr->rowPtr[i]; j < csr->rowPtr[i + 1]; j++) {
            result[i] += csr->values[j] * vec[csr->colIndex[j]];
        }
    }
}

int my_sparse(const double *dense_matrix, const double *vec, double *result, int size) {
   
    CSRMatrix *csr_matrix = convert_to_csr(dense_matrix, size, size);

   
    csr_spmv(csr_matrix, vec, result);

    free(csr_matrix->values);
    free(csr_matrix->colIndex);
    free(csr_matrix->rowPtr);
    free(csr_matrix);

    return 0;
}
