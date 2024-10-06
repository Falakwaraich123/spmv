To compile this code you first need to install gcc 
Command to compile this code 
gcc -o spmv spmv.c my_dense.c my_sparse.c timer.c -lopenblas -lm
