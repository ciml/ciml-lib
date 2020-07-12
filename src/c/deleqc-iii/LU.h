#ifndef LU_H_INCLUDED
#define LU_H_INCLUDED

void PrintUnsIntVector(unsigned int *vec, unsigned int c_size);
void PrintDoubleVector(double *vec, unsigned int c_size);
void lu(double **A, int *p, int n);
double* lu_solve(double **A, int *p, double *b, int n);
void PrintUpperMatrix(double **mat, int *p, int r_size, int c_size);
void PrintLowerMatrix(double **mat, int *p, int r_size, int c_size);

#endif // LU_H_INCLUDED
