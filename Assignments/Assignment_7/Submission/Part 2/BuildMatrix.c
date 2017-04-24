#include<stdlib.h>
double ** BuildMatrix(int Nrows, int Ncols) {
    int n;
    double **A = (double **) calloc(Nrows, sizeof(double *));
    A[0] = (double *) calloc(Nrows * Ncols, sizeof(double));
    for (n = 1; n < Nrows; ++n) {
        A[n] = A[n - 1] + Ncols;
    }
    return A;
}
