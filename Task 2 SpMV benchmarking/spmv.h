// spmv.h
int my_dense(const unsigned int n, const double mat[], double vec[], double result[]);
int my_sparse(const double *dense_matrix, const double *vec, double *result, int size);
int my_coo(const double *dense_matrix, const double *vec, double *result, int size);
int my_csc(const double *dense_matrix, const double *vec, double *result, int size);
